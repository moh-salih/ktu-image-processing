#include "MainWindow.h"

#include <algorithm>
#include <vector>

#include <QtCharts>

/* filters -------------------------------------------------------------- */
#include "core/filters/Median.h"
#include "core/filters/Mean.h"
#include "core/filters/Gaussian.h"
#include "core/filters/Sharpen.h"
#include "core/filters/Sobel.h"
#include "core/filters/Prewitt.h"
#include "core/filters/HistogramEqualization.h"
#include "core/filters/ContrastStretch.h"
#include "core/filters/ThresholdManual.h"
#include "core/filters/ThresholdOtsu.h"
#include "core/filters/ThresholdKapur.h"
#include "core/filters/Dilation.h"
#include "core/filters/Erosion.h"
#include "core/filters/Skeletonization.h"
#include "core/filters/Mirror.h"
#include "core/filters/Shift.h"
#include "core/filters/Rotate.h"
#include "core/filters/Centroid.h"

#include "MainWindow.h"

#include <QtCharts>   // keep if you use charts

#include "MainWindow.h"

#include <QtCharts>

/* ====================== ctor ====================== */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initUi();           
    setWindowTitle("Image Processing: MOHAMMED JABBAR SALIH - 392205");
    setWindowIcon(QIcon((":/res/logos/ktu.png")));
}

/* =================== top-level UI =================== */
void MainWindow::initUi()
{
    buildCentralWidget();   // image pane
    buildParamDock();       // docked parameter panel
    buildMenus();
    buildToolbar();
    applyStyles();

    resize(1000, 650);
    setWindowTitle(QObject::tr("Image Processing"));
}

/* ---------------- central widget (left side) ---------------- */
void MainWindow::buildCentralWidget()
{
    auto* central = new QWidget(this);
    central->setObjectName("mainPanel");  

    auto* root = new QHBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0); // tight fit
    root->setSpacing(0);                 

    buildImagePane();                 
    root->addWidget(m_imageWidget, 1);

    setCentralWidget(central);
}


/* ---------------- images + small histograms ---------------- */
void MainWindow::buildImagePane()
{
    m_originalLabel  = new QLabel;
    m_processedLabel = new QLabel;
    attachSaveContextMenu(m_originalLabel,  [this] { return m_originalImage;  });
    attachSaveContextMenu(m_processedLabel, [this] { return m_processedImage; });


    m_originalLabel ->setAlignment(Qt::AlignCenter);
    m_processedLabel->setAlignment(Qt::AlignCenter);

    m_origHistView = new QChartView(new QChart, this);
    m_procHistView = new QChartView(new QChart, this);
    m_origHistView->setMaximumHeight(160);
    m_procHistView->setMaximumHeight(160);

    auto *leftCol  = new QVBoxLayout;
    auto *rightCol = new QVBoxLayout;
    leftCol ->addWidget(m_originalLabel);
    leftCol ->addWidget(m_origHistView);
    rightCol->addWidget(m_processedLabel);
    rightCol->addWidget(m_procHistView);

    auto *imgLayout = new QHBoxLayout;
    imgLayout->addLayout(leftCol , 1);
    imgLayout->addLayout(rightCol, 1);

    m_imageWidget = new QWidget(this);
    m_imageWidget->setLayout(imgLayout);
}

/* ---------------- parameter dock (right side) ---------------- */
void MainWindow::buildParamDock(){
    m_paramPanel  = new QWidget(this);
    m_paramLayout = new QVBoxLayout(m_paramPanel);
    m_paramLayout->setContentsMargins(4,4,4,4);
    m_paramLayout->setAlignment(Qt::AlignTop);
    m_paramPanel->setObjectName(QObject::tr("paramPane"));

    m_paramDock = new QDockWidget(QObject::tr("Filter Parameters"), this);
    m_paramPanel->setMinimumWidth(280);

    m_paramDock->setWidget(m_paramPanel);
    m_paramDock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    m_paramDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    addDockWidget(Qt::RightDockWidgetArea, m_paramDock);
}

/* ---------------- menus ---------------- */
void MainWindow::buildMenus()
{
    /* ---- File menu ---- */
    m_fileMenu = menuBar()->addMenu(tr("&File"));

    m_openAct = m_fileMenu->addAction(tr("Open Image…"),
                                      this, &MainWindow::openImage);
    m_openAct->setShortcut(QKeySequence::Open);

    m_saveAct = m_fileMenu->addAction(tr("Save Processed Image…"),
                                      this, &MainWindow::saveImage);
    m_saveAct->setShortcut(QKeySequence::Save);

    /* ---- Help menu ---- */
    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_aboutAct = m_helpMenu->addAction(tr("About"), this, &MainWindow::showAbout);
}


/* ---------------- toolbar (unchanged) ---------------- */
void MainWindow::buildToolbar()
{
    auto *tb = addToolBar(QObject::tr("Filters"));
    tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tb->setMovable(false);  tb->setFloatable(false);

    auto add = [tb,this](QString label, QString key, QString icon=QString()) {
        QAction *a = tb->addAction(label, [=]{ applyFilter(key); });
        if (!icon.isEmpty()) a->setIcon(QIcon(icon));
    };

    add("Mean","Mean",":/res/icons/mean.png");
    add("Median","Median",":/res/icons/median.png");
    add("Gaussian","Gaussian",":/res/icons/gaussian.png");
    add("Sharpen","Sharpen",":/res/icons/sharpen.png");  tb->addSeparator();

    add("Sobel","Sobel",":/res/icons/sobel.png");
    add("Prewitt","Prewitt",":/res/icons/prewitt.png");  tb->addSeparator();

    add("Hist Eq.","HistogramEqualization",":/res/icons/histogram.png");
    add("Contrast","ContrastStretch",":/res/icons/contrast.png"); tb->addSeparator();

    add("Manual","ThresholdManual",":/res/icons/manual.png");
    add("Otsu","ThresholdOtsu",":/res/icons/otsu.png");
    add("Kapur","ThresholdKapur",":/res/icons/kapur.png"); tb->addSeparator();

    add("Rotate","Rotate",":/res/icons/rotate.png");
    add("Shift","Shift",":/res/icons/shift.png");
    add("Mirror","Mirror",":/res/icons/mirror.png"); tb->addSeparator();

    add("Dilate","Dilation",":/res/icons/dilation.png");
    add("Erode","Erosion",":/res/icons/erosion.png");
    add("Skeleton","Skeletonization",":/res/icons/skeletonization.png"); tb->addSeparator();

    add("Centroid","Centroid",":/res/icons/centroid.png");
}

/* ---------------- stylesheet ---------------- */
void MainWindow::applyStyles()
{
    setStyleSheet(R"(
        QWidget#paramPane {
            background-color: #f9f9f9;
            border-left: 1px solid #c0c0c0;
        }

        QWidget#mainPanel {
            background-color: #ffffff;   /* ✅ gives solid look */
            border: 1px solid #dcdcdc;   /* subtle frame */
        }

        QLabel[role='title'] {
            font-weight: bold;
            font-size: 14px;
        }

        QGroupBox {
            font-weight: 600;
            margin-top: 10px;
        }

        QSpinBox, QDoubleSpinBox, QComboBox {
            min-width: 80px;
        }

        QGroupBox {
            font-weight: bold;
            border: 1px solid #ccc;
            border-radius: 6px;
            margin-top: 20px;
        }

        QGroupBox:title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 0 9px;
            background-color: #f5f5f5;
        }

        QFormLayout {
            padding: 4px;
        }

        QSpinBox {
            min-width: 80px;
        }

    )");
}

/* =====================================================
 *  All remaining methods (applyFilter, openImage, etc.)
 *  stay exactly the same except for the typo fix below.
 * ===================================================== */

void MainWindow::applyFilter(const QString& key)
{
    if (m_originalImage.isNull()) return;
    preparePanel();

    auto &filter = m_proc.find<core::filters::Filter>(key.toStdString());

    auto refresh = [this,key]{
        m_processedImage = m_proc.apply(key.toStdString(), m_originalImage.convertToFormat(QImage::Format_Grayscale8)); 
        updateProcessedLabel();
    };
    filter.buildUI(m_paramPanel, refresh);
    refresh();
    m_paramPanel->update();
}

void MainWindow::updateProcessedLabel()
{
    showImage(m_originalLabel,  m_originalImage);
    showImage(m_processedLabel, m_processedImage);

    updateHistogram(m_originalImage.convertToFormat(QImage::Format_Grayscale8), m_origHistView); // ✅
    updateHistogram(m_processedImage.convertToFormat(QImage::Format_Grayscale8), m_procHistView); // ✅
}

void MainWindow::attachSaveContextMenu(QLabel* label, std::function<QImage()> imageGetter)
{
    label->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(label, &QLabel::customContextMenuRequested,
            this, [this, label, imageGetter](const QPoint& pos)
    {
        const QImage& img = imageGetter();          // reference to current image
        if (img.isNull()) return;                   // nothing to save

        QMenu menu(label);
        QAction* saveAct = menu.addAction(QObject::tr("Save image as…"));

        if (menu.exec(label->mapToGlobal(pos)) == saveAct) {
            const QString fn = QFileDialog::getSaveFileName(
                this,
                QObject::tr("Save image"),
                QDir::homePath(),
                allSupportedImageFilter(true));

            if (!fn.isEmpty() && !img.save(fn))
                QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Failed to save image."));
        }
    });
}


void MainWindow::showAbout()
{
    QDialog dlg(this);
    dlg.setWindowTitle(QObject::tr("About This Project"));

    QVBoxLayout* layout = new QVBoxLayout(&dlg);

    QLabel* logo = new QLabel();
    logo->setPixmap(QPixmap(":/res/logos/ktu.png").scaled(160, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logo->setAlignment(Qt::AlignCenter);

    QLabel* text = new QLabel(
        QObject::tr("<h3 style='margin-bottom:8px;'>Image Processing Project</h3>"
        "<p>This application was implemented for the <b>Image Processing</b> course "
        "at Karadeniz Technical University during the 2024–2025 academic term.</p>"
        "<p>All filters and operations were implemented manually without relying on "
        "built-in image processing libraries.</p>"
        "<p style='margin-top:12px;'>"
        "<b>Developer:</b> Mohammed Jabbar Salih<br>"
        "<b>GitHub:</b> <a href='https://github.com/moh-salih'>github.com/moh-salih</a>"
        "</p>")
    );
    text->setOpenExternalLinks(true);
    text->setWordWrap(true);
    text->setAlignment(Qt::AlignCenter);

    QPushButton* okBtn = new QPushButton("OK");
    QObject::connect(okBtn, &QPushButton::clicked, &dlg, &QDialog::accept);

    layout->addWidget(logo);
    layout->addWidget(text);
    layout->addWidget(okBtn, 0, Qt::AlignCenter);

    dlg.resize(440, 360);
    dlg.exec();
}

void MainWindow::preparePanel(){
    // Remove all items from the layout
    QLayoutItem* item;
    while (item = m_paramLayout->takeAt(0)) {
        if (item->widget()) {
            // Hide and delete the widget
            item->widget()->hide();
            item->widget()->deleteLater();
        }
        delete item;
    }
    
}



void MainWindow::updateHistogram(const QImage& img, QChartView* view){
    if (img.isNull() || !view) return;

    int hist[256] = {0};
    const uchar* p = img.constBits();
    const int n   = img.width() * img.height();
    for (int i = 0; i < n; ++i) ++hist[p[i]];

    auto* series = new QLineSeries;
    for(int i = 0; i < 256; ++i) series->append(i, hist[i]);

    auto* chart = view->chart();
    chart->removeAllSeries();
    chart->addSeries(series);
    chart->legend()->hide();

    chart->createDefaultAxes();
    chart->axisX()->setRange(0, 255);
    chart->axisY()->setTitleText(QObject::tr("Count"));
    chart->axisX()->setTitleText(QObject::tr("Intensity"));

    view->setRenderHint(QPainter::Antialiasing, false); // faster
}

// --------------------------------------------------------------------
// 1. helper: returns "Images (*.png *.jpg *.jpeg *.tiff …)"
// --------------------------------------------------------------------
QString MainWindow::allSupportedImageFilter(bool forWrite)
{
    const auto fmts = forWrite
        ? QImageWriter::supportedImageFormats()
        : QImageReader::supportedImageFormats();

    QStringList patterns;
    patterns.reserve(fmts.size());
    for (const QByteArray &fmt : fmts)
        patterns << "*." + QString::fromLatin1(fmt).toLower();

    return "Images (" + patterns.join(' ') + ')';
}


void MainWindow::openImage(){
    auto path = QDir::current();
    path.cdUp();

    const QString fn = QFileDialog::getOpenFileName(this, QObject::tr("Open image"), path.currentPath(), allSupportedImageFilter());
    if (fn.isEmpty()) return;

    QImageReader r(fn);  r.setAutoTransform(true);
    const QImage img = r.read();
    if (img.isNull()) {
        QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Failed to load image."));
        return;
    }

    m_originalImage = img;
    m_processedImage = img;
    showImage(m_originalLabel, m_originalImage);
    showImage(m_processedLabel, m_processedImage);
}

void MainWindow::saveImage()
{
    QDir path = QDir::current();
    path.cdUp();

    if (m_processedImage.isNull()) return;
    const QString fn = QFileDialog::getSaveFileName(this, QObject::tr("Save image"), path.currentPath(), allSupportedImageFilter(true));
    if (fn.isEmpty()) return;
    if (!m_processedImage.save(fn))
        QMessageBox::critical(this, QObject::tr("Error"), QObject::tr("Failed to save image."));
}

void MainWindow::showHistogramDialog(const QImage& img, const QString& title)
{
    if (img.isNull()) return;

    int hist[256]{};
    const uchar* p = img.constBits();
    const int n = img.width() * img.height();
    for (int i = 0; i < n; ++i) ++hist[p[i]];

    auto* series = new QLineSeries();
    for (int i = 0; i < 256; ++i) series->append(i, hist[i]);

    auto* chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle(title);

    auto* axX = qobject_cast<QValueAxis*>(chart->axes(Qt::Horizontal).first());
    auto* axY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    if (axX) axX->setRange(0, 255);
    if (axX) axX->setTitleText(QObject::tr("Intensity"));
    if (axY) axY->setTitleText(QObject::tr("Count"));

    chart->addAxis(axX, Qt::AlignBottom);
    chart->addAxis(axY, Qt::AlignLeft);
    series->attachAxis(axX);
    series->attachAxis(axY);

    QDialog dlg(this);
    dlg.setWindowTitle(title);
    auto* lay = new QVBoxLayout(&dlg);
    lay->addWidget(new QChartView(chart));
    dlg.resize(820, 420);
    dlg.exec();
}

void MainWindow::showImage(QLabel* lbl, const QImage& img)
{
    if (!lbl) return;

    if (img.isNull()) {
        lbl->clear();
        return;
    }

    QPixmap px = QPixmap::fromImage(img);
    if (px.isNull()) { 
        lbl->clear();
        return;
    }

    lbl->setPixmap(px.scaled(lbl->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}


void MainWindow::showHistogram() {
    showHistogramDialog(m_processedImage, QObject::tr("Grayscale Histogram"));
}

