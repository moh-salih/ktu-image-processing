#ifndef CORE_QMAT_H
#define CORE_QMAT_H

#include <QImage>
#include <stdexcept>
#include <algorithm>
#include <iterator>

/* ------------------------------ QMat ---------------------------------- */
/* a **lightweight view** over a QImage                                  */

class QMat {
public:
    /* public-facing ctors – **never** construct empty views ------------ */
    explicit QMat(QImage &img)              : m_img(&img) { init(); }
    explicit QMat(const QImage &img)        : m_img(const_cast<QImage*>(&img)) { init(); }

    /* basic geometry ---------------------------------------------------- */
    int rows()       const { return m_img->height(); }
    int cols()       const { return m_img->width();  }
    int height()     const { return rows(); }
    int width()      const { return cols(); }
    int channels()   const { return m_channels; }
    int step()       const { return m_step; }
    bool empty()     const { return m_img->isNull(); }
    QImage::Format format() const { return m_img->format(); }

    /* bounds helpers ---------------------------------------------------- */
    bool inBounds(int y,int x) const { return y>=0 && y<rows() && x>=0 && x<cols(); }

    /* raw access -------------------------------------------------------- */
    uchar*       ptr(int y)       { checkRow(y);  return m_img->bits() + y*m_step; }
    const uchar* ptr(int y) const { checkRow(y);  return m_img->constBits() + y*m_step; }

    /* ------------------------------------------------------------------ */
    struct Pixel { uchar r{},g{},b{},a{255};
        static Pixel fromQRgb(QRgb c){ return{uchar(qRed(c)),uchar(qGreen(c)),uchar(qBlue(c)),uchar(qAlpha(c))}; }
        QRgb toQRgb() const { return qRgba(r,g,b,a); }
    };

    /* high-level per-pixel --------------------------------------------- */
    Pixel  getPixel(int y,int x) const {
        checkYX(y,x);
        const uchar* p = ptr(y)+x*m_channels;
        switch (m_img->format()){
            case QImage::Format_Grayscale8: return {p[0],p[0],p[0],255};
            case QImage::Format_RGB888:     return {p[0],p[1],p[2],255};
            case QImage::Format_RGBA8888:   return {p[0],p[1],p[2],p[3]};
            case QImage::Format_RGB32:
            case QImage::Format_ARGB32:     return Pixel::fromQRgb(*reinterpret_cast<const QRgb*>(p));
            default: throw std::runtime_error("QMat: unsupported format");
        }
    }
    void   setPixel(int y,int x,const Pixel &px){
        checkYX(y,x);  uchar* p = ptr(y)+x*m_channels;
        switch (m_img->format()){
            case QImage::Format_Grayscale8: p[0]=px.r; break;
            case QImage::Format_RGB888:     p[0]=px.r; p[1]=px.g; p[2]=px.b; break;
            case QImage::Format_RGBA8888:   p[0]=px.r; p[1]=px.g; p[2]=px.b; p[3]=px.a; break;
            case QImage::Format_RGB32:
            case QImage::Format_ARGB32:     *reinterpret_cast<QRgb*>(p)=px.toQRgb(); break;
            default: throw std::runtime_error("QMat: unsupported format");
        }
    }

    /* convenience – many filters were still using at()/clone() etc. ---- */
    Pixel  at(int y,int x)  const { return getPixel(y,x); }      // read-only
    QMat   copy()           const { return QMat( m_img->copy() );}  // deep copy
    QMat   clone()          const { return copy(); }
    QMat   createSameSize() const { return QMat( m_img->copy() ); }

    /* pixel-wise iteration (modifiable) --------------------------------- */
    template<class F> void forEachPixel(F&& fn){
        for(int y=0;y<rows();++y)
            for(int x=0;x<cols();++x){
                Pixel px = getPixel(y,x);
                fn(y,x,px);                          // user mutates local copy
                setPixel(y,x,px);                    // write-back
            }
    }
    template<class F> void forEachKernel(int cy,int cx,int k,F&& fn) const{
        const int r=k/2;
        for(int y=cy-r;y<=cy+r;++y)
            for(int x=cx-r;x<=cx+r;++x)
                if(inBounds(y,x)) fn(getPixel(y,x));
    }

    /* convert back ------------------------------------------------------ */
    QImage toQImage() const { return m_img->copy(); }

private:
    QImage* m_img{};
    int     m_step     =0;
    int     m_channels =0;

    void init(){
        if(m_img->isNull()) throw std::runtime_error("QMat: null image");
        m_step = m_img->bytesPerLine();
        switch(m_img->format()){
            case QImage::Format_Grayscale8: m_channels=1; break;
            case QImage::Format_RGB888:     m_channels=3; break;
            case QImage::Format_RGBA8888:
            case QImage::Format_RGB32:
            case QImage::Format_ARGB32:     m_channels=4; break;
            default: throw std::runtime_error("QMat: unsupported format");
        }
    }
    void checkRow(int y) const { if(y<0||y>=rows()) throw std::out_of_range("QMat row"); }
    void checkYX (int y,int x) const { if(y<0||y>=rows()||x<0||x>=cols()) throw std::out_of_range("QMat pixel"); }
};

#endif // CORE_QMAT_H
