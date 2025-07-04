#pragma once
#include <QtWidgets>
#include <QtCharts>

#include "core/ImageProcessor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void openImage();
    void saveImage();
    void showAbout();


private:                      /* ─── UI builders ─── */
    void initUi();            // top-level entry
    void buildCentralWidget();// images pane
    void buildImagePane();    // two images + histograms
    void buildParamDock();    // right-hand dock panel
    void buildMenus();
    void buildToolbar();
    void applyStyles();

private:                      /* ─── helpers (unchanged) ─── */
    void applyFilter(const QString& key);
    void showHistogram();
    void showHistogramDialog(const QImage& img, const QString& title = "Histogram");
    void showImage(QLabel* lbl, const QImage& img);
    void updateProcessedLabel();
    void updateHistogram(const QImage& img, QChartView* view);
    void preparePanel();
    void resizeEvent(QResizeEvent*) override { updateProcessedLabel(); }
    void attachSaveContextMenu(QLabel* label, std::function<QImage()> imageGetter);
    QString allSupportedImageFilter(bool forWrite = false);

private:                      /* ─── widgets ─── */
    QWidget*     m_imageWidget   = nullptr;  // images container
    QLabel*      m_originalLabel = nullptr;
    QLabel*      m_processedLabel= nullptr;
    QChartView*  m_origHistView  = nullptr;
    QChartView*  m_procHistView  = nullptr;

    QWidget*     m_paramPanel    = nullptr;  // hosted inside the dock
    QVBoxLayout* m_paramLayout   = nullptr;
    QDockWidget* m_paramDock     = nullptr;

    /* images */
    QImage m_originalImage;
    QImage m_processedImage;


    // pointers we need to re-label
    QMenu   *m_fileMenu  = nullptr;
    QMenu   *m_helpMenu  = nullptr;
    QAction *m_openAct   = nullptr;
    QAction *m_saveAct   = nullptr;
    QAction *m_aboutAct  = nullptr;

    /* core */
    core::ImageProcessor& m_proc = core::ImageProcessor::instance();
};
