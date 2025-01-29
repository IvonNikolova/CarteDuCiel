#include "carteduciel.h"
#include "ui_carteduciel.h"

#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>


using namespace std;

/* constructor */
CarteDuCiel::CarteDuCiel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CarteDuCiel)
{
    ui->setupUi(this);

    /* set main window's name */
    setWindowTitle("Carte Du Ciel Software");

    /* set limit (about the uploaded pictures) */
    QImageReader::setAllocationLimit(0); // Sets the allocation limit to mbLimit megabytes. Images that would require a QImage memory allocation above this limit will be rejected. If mbLimit is 0, the allocation size check will be disabled.
                                         // Sources: https://doc-snapshots.qt.io/qt6-dev/qimagereader.html#setAllocationLimit ; https://stackoverflow.com/questions/71458968/pyqt6-how-to-set-allocation-limit-in-qimagereader
}// CarteDuCiel

CarteDuCiel::~CarteDuCiel()
{
    delete ui;
    delete hchart;
    delete horchart;
    delete verchart;

}// ~CarteDuCiel

void CarteDuCiel::on_pushB_uploadImg_clicked()
{
   // Reset some displayed information
    ui->textB_printHisto->clear();
    ui->textB_printHisto_Occurences->clear();

    // Get the file image from
    filename = QFileDialog::getOpenFileName(this,tr("Choose"),"",tr("Images (*.tiff *.tif *.png *.jpg *.gif *.jpeg)"));

    // Check if it is a valid file
    if (QString::compare(filename, QString()) != 0)
    {
        if (sourceImage.load(filename) && sourceImage0.load(filename))
        {
            //Display the QImage in GraphicsView
            item0 = new QGraphicsPixmapItem(QPixmap::fromImage(this->sourceImage0)); //ORIGINAL IMAGE
            scene0 = new QGraphicsScene(this);
            ui->graphics_view0->setScene(scene0); //graphicsView is the name of the window in the UI file, we set the scene into the graphisview
            scene0->addItem(item0); //add Item into the scene


            item = new QGraphicsPixmapItem(QPixmap::fromImage(this->sourceImage)); //AFTER TRANSFORMATION IMAGE
            scene = new QGraphicsScene(this);
            ui->graphics_view1->setScene(scene);
            scene->addItem(item);
        }
        else
        {
            // Error handling
            QMessageBox::critical(this,"Error","Cannot load the wanted image!");
            qDebug("Cannot load the image!");
        }
    }

    //About the loaded image format
    // Sources: https://doc.qt.io/qt-6/qimage.html
    QString info1,info2,info3;

    if(sourceImage0.format() == QImage::Format_Mono
       || sourceImage0.format() == QImage::Format_MonoLSB)
    {
       info3 =  "The image is stored using 1-bit per pixel.";
    }
    else if(sourceImage0.format() == QImage::Format_Indexed8
            || sourceImage0.format() == QImage::Format_Alpha8
            ||sourceImage0.format() == QImage::Format_Grayscale8)
    {
       info3 = "The image is stored using 8-bit indexes of the given format.";
    }
    else if(sourceImage0.format() == QImage::Format_Grayscale16
            || sourceImage0.format() == QImage::Format_ARGB4444_Premultiplied
            || sourceImage0.format() == QImage::Format_RGB444
            || sourceImage0.format() == QImage::Format_RGB555
            || sourceImage0.format() == QImage::Format_RGB16)
    {
       info3 = "The image is stored using 16-bit indexes of the given format.";
    }
    else if(sourceImage0.format() == QImage::Format_RGB32
            || sourceImage0.format() == QImage::Format_ARGB32
            || sourceImage0.format() == QImage::Format_ARGB32_Premultiplied
            || sourceImage0.format() == QImage::Format_RGBX8888
            || sourceImage0.format() == QImage::Format_RGBA8888
            || sourceImage0.format() == QImage::Format_RGBA8888_Premultiplied
            || sourceImage0.format() == QImage::Format_BGR30
            || sourceImage0.format() == QImage::Format_A2BGR30_Premultiplied
            || sourceImage0.format() == QImage::Format_RGB30
            || sourceImage0.format() == QImage::Format_A2RGB30_Premultiplied
            || sourceImage0.format() == QImage::Format_RGBX32FPx4
            || sourceImage0.format() == QImage::Format_RGBA32FPx4
            || sourceImage0.format() == QImage::Format_RGBA32FPx4_Premultiplied
           )//32
    {
       info3 = "The image is stored using a 32-bit of the given format.";
    }
    else if(sourceImage0.format() == QImage::Format_Invalid) {
        info3 = "-";
    }

    info1 = "Image width: " + QString::number(sourceImage0.width());
    info1 += "\nImage height: " + QString::number(sourceImage0.height());
    QMetaEnum metaEnum = QMetaEnum::fromType<QImage::Format>(); // Sources: https://forum.qt.io/topic/112111/show-qimage-information-on-qtextbrowser
    info2 += metaEnum.valueToKey(sourceImage0.format());
    ui->textB_infoLoadedImage->setText( "Image path: " + filename + '\n' + info1 + "\nFormat: " + info2 + "\nAbout the bits: " + info3);
}// on_pushB_uploadImg_clicked 


void CarteDuCiel::on_pushB_deleteImg_clicked()
{
    if (!sourceImage.isNull() || !sourceImage0.isNull())
    {
        sourceImage = QImage();
        sourceImage0 = QImage();
        sourceImage1 = QImage();

        scene0->clear(); //this removes the scene of the Graphicsview
        ui->graphics_view0->scene()->clear();  // Sources: https://forum.qt.io/topic/15075/solved-how-to-reset-qgraphicsview-qgraphicsscene-to-display-new-image-without-overlapping-with-previous-image/11
        scene->clear();
        ui->graphics_view1->scene()->clear();
        ui->graphics_view2->scene()->clear();
        ui->textB_infoLoadedImage->clear();
        ui->textB_printHisto->clear();
        ui->textB_printHisto_Occurences->clear();
    }
    else
    {
       QMessageBox::information(this, "Still no image!", "There is no image to be deleted!",QMessageBox::Cancel);
    }
}// on_pushB_deleteImg_clicked


void CarteDuCiel::on_pushB_saveImg_clicked()
{
    // Get the home directory (macOS/Linux) or USERPROFILE (Windows)
        const char* homeDir = std::getenv("HOME");

        if (!homeDir) {
            std::cerr << "Could not get home directory!" << std::endl;
            return;
        }

    // Construct the path for ProjectFolder and the image file
    QString desktopPath = QString(homeDir) + "/Desktop";
    QString folderPath = desktopPath + "/CarteDuCiel_Outputs";

    // Create the folder if it doesn't exist
    QDir dir(folderPath);
    if (!dir.exists()) {
        if (dir.mkpath(folderPath)) {
            std::cout << "Directory created successfully: " << folderPath.toStdString() << std::endl;
        } else {
            std::cerr << "Failed to create directory!" << std::endl;
            return;
        }
    }

    // Construct the full file path for the image
    QString filePath1 = QDir(folderPath).filePath("OtsuImage.jpg");
    QString filePath2 = QDir(folderPath).filePath("originalImage.jpg");
    QString filePath3 = QDir(folderPath).filePath("projectionsImage.jpg");
    QString filePath4 = QDir(folderPath).filePath("sobelImage.jpg");

    sourceImage.save(filePath1);
    sourceImage0.save(filePath2);
    sourceImage1.save(filePath3);
    sobelImage.save(filePath4);
}// on_pushB_saveImg_clicked

void CarteDuCiel::on_pushB_applyAlgorithms_clicked()
{

    QTextStream(stdout) << "Filename: " << filename << "\n\n";
    // 1st approach '000008'
        std::string inputPath = filename.toStdString(); // Convert QString to std::string

        // Find the position of the last underscore
        std::size_t underscorePos = inputPath.rfind('_');

        // Find the position of the last period (.)
        std::size_t dotPos = inputPath.rfind('.');

        if (underscorePos != std::string::npos && dotPos != std::string::npos && underscorePos < dotPos) {
            // Extract the substring between the last underscore and the file extension
            std::string numberPart = inputPath.substr(underscorePos + 1, dotPos - underscorePos - 1);
//            std::cout << "Extracted string: " << numberPart << std::endl;
        } else {
            std::cerr << "The format of the input path is incorrect." << std::endl;
        }

    //2nd approach ROB033_000008
        // Use QFileInfo to extract the file name without path
        QFileInfo fileInfo(filename);
        QString baseName = fileInfo.completeBaseName(); // This gets the file name without the extension

        // Convert to std::string if needed
        std::string plateName = baseName.toStdString();

//        std::cout << "Filename: " << plateName << std::endl;

     // Check the format of the input image and convert it if needed
    if (this->sourceImage.format() != QImage::Format_Grayscale8)
    {
       int r = sourceImage.height();
       int c = sourceImage.width();

        QImage grscl_image;
        grscl_image = this->sourceImage.convertToFormat(QImage::Format_Grayscale8);
        this->sourceImage = grscl_image;

          /** Sources: 
           * https://stackoverflow.com/questions/42316844/convert-qimageicon-to-grayscale-format-while-keeping-background
           * https://stackoverflow.com/questions/27949569/convert-a-qimage-to-grayscale
           * https://stackoverflow.com/questions/14144204/image-converter-in-qt-colored-image-to-black-white
           */ 
    }



   if (this->sourceImage.format() == QImage::Format_Grayscale8)// || this->sourceImage.format() == QImage::Format_Grayscale16 )
   {

//--------------------------------------Reseau Segmentation--------------------------------------------------------------
//--------------------------------------Otsu Binarization--------------------------------------------------------------

        // Apply the Otsu's algorithm
        OtsuAlgorithm otsu;

        // Total number of pixels of the uploaded image
        int numb_pix = this->sourceImage.height() * this->sourceImage.width();

        // Init histogram and count occurences
        otsu.calcHisto(this->sourceImage, histo);


        QString histo_occurences;
        for (int i = 0; i <= INTENS_MAX; i++)
        {
                if (histo[i] != 0)
                {
                     histo_occurences +=  QString::number(i) + " | " + QString::number(histo[i],'f', 0) + '\n'; //Sources: https://stackoverflow.com/questions/5940846/convert-a-double-to-a-qstring
                }
        }
        ui->textB_printHisto_Occurences->setText("Total number of pixels: " + QString::number(numb_pix) + '\n' + '\n'  + histo_occurences);

        // Print out histogram result
        //  otsu.printHisto(histo);
        // Copy the content of the histogram for the histogram chart before changing its values to the calaculated probability
        copy_histo.assign(std::begin(histo), std::end(histo));
        //Calculate probability
        otsu.calcProb(this->sourceImage, histo);
        copy_prob_histo.assign(std::begin(histo), std::end(histo));
        // Threshold calculations
        th = otsu.otsu(histo, cumulative_sum, between_class_variance);
        otsu.threshold(this->sourceImage, th);

        // Print probability result
        QString histo_prob;
        for (int i = 0; i <= INTENS_MAX; i++)
        {
            if (histo[i] != 0)
            {
                histo_prob +=  QString::number(i) + " | " + QString::number(histo[i],'f', 8) + '\n';
            }
            else
            {
                histo_prob += QString::number(i) + " | " + QString::number(histo[i]) + '\n';
            }
         }
         ui->textB_printHisto->setText( "Threshold: " + QString::number(th) + '\n' + '\n' +  histo_prob);



//        cout << endl << endl;

//----------------------------------Projections------------------------------------------------------------------
        ProjectionsCalculator prj;

        // Compute horizontal projections and copy their contents into new variables
        prj.horizontalPrjctn(this->sourceImage, th, horizontal_projection, this->sourceImage.height(), this->sourceImage.width());
//        cout << "\nHORIZONTAL PROJECTION" << '\n';
        //prj.printProj(horizontal_projection);



        QString homeDir = QDir::homePath();  // Safer way to get the home directory
        QString desktopPath = homeDir + "/Desktop/";
        QString folderPath = desktopPath + "CarteDuCiel_Outputs/";
        QString fullPath0 = QDir(folderPath).filePath("horizontal_projection.txt");

        // Ensure the directory exists
        QDir().mkpath(folderPath);

         // Open the file for writing
         std::ofstream file1(fullPath0.toStdString());
         // Check if the file is opened successfully
         if (!file1.is_open())
         {
             cout << "Error: Unable to open file for writing." << endl;
             exit(1);
         }
         // Write the vector data to the file
         for (int i = 0; i < horizontal_projection.size(); i++)
         {
             file1 << horizontal_projection[i];
             if(i < horizontal_projection.size() - 1)
             {
                 file1 << ", ";
             }
         }
         // Close the file
         file1.close();
//         cout << "Data successfully written to file on desktop." << endl;


        horizontal_projection2 = horizontal_projection;
        horizontal_projection0 = horizontal_projection;
        vector<double>  horizontal_projection_peaks = horizontal_projection;

        QString h_proj;
        for (int i = 0; i <= horizontal_projection.size(); i++)
        {
            h_proj +=  QString::number(i) + " | " + QString::number(horizontal_projection[i],'f',0) + '\n';
        }
        ui->textB_horizontal_proj->setText(h_proj);


        // Compute vertical projections and copy their contents into new variables
        vertical_projection = prj.verticalPrjctn(this->sourceImage, th);
//        cout << "\nVERTICAL PROJECTION" << '\n';
        //prj.printProj(vertical_projection);

        fullPath0 = QDir(folderPath).filePath("vertical_projection.txt");

        // Open the file for writing
        ofstream file2(fullPath0.toStdString());
        // Check if the file is opened successfully
        if (!file2.is_open())
        {
            cout << "Error: Unable to open file for writing." << endl;
            exit(1);
        }
        // Write the vector data to the file
        for (int i = 0; i < vertical_projection.size(); i++)
        {
            file2 << vertical_projection[i];
            if(i < vertical_projection.size() - 1)
            {
                file2 << ", ";
            }
        }
        // Close the file
        file2.close();
//        cout << "Data successfully written to file on desktop." << endl;


        vertical_projection2 = vertical_projection;
        vertical_projection0 = vertical_projection;
        vector<double> vertical_projection_peaks = vertical_projection;
        QString v_proj;
        for (int i = 0; i <= vertical_projection.size(); i++)
        {
                v_proj +=  QString::number(i) + " | " + QString::number(vertical_projection[i],'f',0) + '\n';
        }
        ui->textB_vertical_proj->setText(v_proj);

//--------------------------------------DERIVATIVES - FIRST AND SECOND--------------------------------------------------------------


        Derivatives der;

        // First derivative calculations of horizontal projection or vertical (choose one!)
        der.first_derivative(horizontal_projection);
        der.first_derivative(vertical_projection);

//        cout << "\n First derivative values of horizontal projection: \n";
        //der.printDerivativeValues(horizontal_projection);
//        cout << "\n First derivative values of vertical projection: \n";
        //der.printDerivativeValues(vertical_projection);

        // Copy the content of the horizontal_projection
        copy_first_derivative_h.assign(std::begin(horizontal_projection), std::end(horizontal_projection));
        copy_first_derivative_v.assign(std::begin(vertical_projection), std::end(vertical_projection));

        // Second derivative calculations of horizontal projection/or/ vertical projection
        der.second_derivative(horizontal_projection2);
        der.second_derivative(vertical_projection2);

//        cout << "\n Second derivative values of horizontal projection: \n";
        //der.printDerivativeValues(horizontal_projection2);
//        cout << "\n Second derivative values of vertical projection: \n";
        //der.printDerivativeValues(vertical_projection2);

         // Copy the content of the horizontal_projection
        copy_second_derivative_h.assign(std::begin(horizontal_projection2), std::end(horizontal_projection2));
        copy_second_derivative_v.assign(std::begin(vertical_projection2), std::end(vertical_projection2));
//        cout << endl << endl;

//--------------------------------------------GAUSSIAN 1D - on HORIZONTAL PROJECTION--------------------------------------------------------
        /*
                We could try to smooth the data with a smoothing filter, in our case, Gaussian 1D filter,
                and then find all values where the value before and after are less than the current value.
                This assumes we want all peaks in the sequence.
        */
        Gaussian1DFilter g1f_h;

        // Call the function to create a Gaussian kernel
        std::vector<double> kernel1 = g1f_h.createGaussianKernel(sigma_h);
        gaussiankernel1 = kernel1;

        // The  signal that has to be filtered - horizontal_projection0
        // Apply the Gaussian filter
        filtered_hp = g1f_h.applyConvolutionWithGaussian1Dfilter(horizontal_projection0, kernel1);
        QString h_proj_gf;
        for (int i = 0; i < filtered_hp.size(); i++)
        {
                h_proj_gf +=  QString::number(i) + " | " + QString::number(filtered_hp[i],'f', 6) + '\n';
        }
        ui->textB_horizontal_proj_gaussian->setText(h_proj_gf);
        copy_gaussian_h.assign(std::begin(filtered_hp), std::end(filtered_hp));

//        //GAUSSIAN on HORIZONTAL PROJECTIONS - Find peaks using smoothed data of horizontal projection values
//                // cout << "\nFiltered data of HORIZONTAL projection" << endl;
//                // g1f_h.print_GaussianValues(output_g1f_h);
//        //GAUSSIAN on VERTICAL PROJECTIONS - Find peaks using smoothed data of vertical projection values
//                // cout << "\nFiltered data of VERTICAL projection" << endl;
//               //g1f_v.print_GaussianValues(output_g1f_v);

//-------------------------------------------GRID LINES DETECTOR - for HORIZONTAL PROJECTION---------------------------------------------------------
        // FROM HORIZONTAL PROJECTIONS
       GridlinesDetector peaks_h;
       peaks_h.findPeaks(filtered_hp, filtered_hp_pos, filtered_hp_peaks, 0);
       //cout << "\nPOS: GAUSSIAN on HORIZONTAL PROJECTIONS" << endl;
       //peaks_h.print(filtered_hp_pos);
       // cout << "\nPEAKS: GAUSSIAN on HORIZONTAL PROJECTIONS" << endl;
       //peaks_h.print(filtered_hp_peaks);



// Step 4.1.) Find the frame peaks and erase them

// Step 4.1.1.) Find the frame peaks
// HORIZONTAL PROJECTIONS
//                cout << "\n\nFRAME PEAKS: FILTERED HORIZONTAL PROJECTIONS" << endl;
// LEFT FRAME PEAK
               // Find left frame peak of horizontal projection
               double max_h_frame1 = *max_element(filtered_hp_peaks.begin(), filtered_hp_peaks.end());
//               cout << "FRAME-1 VALUE (max_h_frame1) = " << max_h_frame1 << endl;
               // Remove left frame peak of horizontal projection
               vector<double> remove_Hframepeak1;
               remove_Hframepeak1 = filtered_hp_peaks;
               remove_Hframepeak1.erase(std::remove(remove_Hframepeak1.begin(), remove_Hframepeak1.end(), max_h_frame1), remove_Hframepeak1.end());
// RIGHT FRAME PEAK
            // Find right frame peak of horizontal projection
               double max_h_frame2 = *max_element(remove_Hframepeak1.begin(), remove_Hframepeak1.end());
//               cout << "FRAME-2 VALUE (max_h_frame2) = " << max_h_frame2 << endl;
            // Remove right frame peak of horizontal projection
               vector<double> remove_Hframepeak2;
               remove_Hframepeak2 = remove_Hframepeak1; //remove_Hframepeak2 is the one without frame peaks
               remove_Hframepeak2.erase(std::remove(remove_Hframepeak2.begin(), remove_Hframepeak2.end(), max_h_frame2), remove_Hframepeak2.end());
// VALUE used in threshold
               // Find min value between within the range given by the two frame peaks
               // Needed for the threshold formula
               double min_h_value = *min_element(remove_Hframepeak2.begin(), remove_Hframepeak2.end());
//               cout << "Minimum horizontal  value: " << min_h_value << endl;

// Step 4.1.2.1.) Find the frame peaks indices using their values
// HORIZONTAL PROJECTIONS
                    // find index of first frame peak INSIDE G.FILTERED HORIZONTAL VECTOR
                    double index_max_h_frame1 = peaks_h.findIndex(filtered_hp, max_h_frame1);
//                    cout << "\nIndex of frame peak 1:" << endl;
//                    cout << index_max_h_frame1 << endl; // 3405.26

                    // find index of second frame peak INSIDE G.FILTERED VECTOR
                    double index_max_h_frame2 = peaks_h.findIndex(filtered_hp, max_h_frame2);
//                    cout << "\nIndex of frame peak 2:" << endl;
//                    cout << index_max_h_frame2 << endl;

                    // order correctly the indices
                    if(index_max_h_frame2 < index_max_h_frame1)
                     {
                        //swap
                         double temp = index_max_h_frame2;
                         index_max_h_frame2 = index_max_h_frame1;
                         index_max_h_frame1 = temp;
//                         cout << "\nAfter swap:\nindex_max_h_frame1 = " << index_max_h_frame1 << endl;
//                         cout << "index_max_h_frame2 = " << index_max_h_frame2 << endl;
                     }

// Step 4.1.2.2.) Find the rest PEAKS indices between the two frame PEAKS indices
// HORIZONTAL PROJECTIONS
                    // find the max value of them all
                    double max_h_pvalue = 0.0;
                    if(max_h_frame1 > max_h_frame2)
                    {
                        max_h_pvalue = max_h_frame1;
                    }
                    else if(max_h_frame1 < max_h_frame2)
                    {
                        max_h_pvalue = max_h_frame2;
                    }
                    double thr_h = (max_h_pvalue - min_h_value)/4.0;
//                    cout << "\nthr_h (max/4) =" << thr_h << endl;

                    for(int i = 0; i < filtered_hp_peaks.size(); i++)
                    {
                        if(index_max_h_frame1 < index_max_h_frame2)
                        {
                            // set boundaries
                            if(filtered_hp_pos[i] > index_max_h_frame1 && filtered_hp_pos[i] < index_max_h_frame2)
                            {
                                middle_pos_hp.push_back(filtered_hp_pos[i]);//indices
                                middle_peaks_hp = peaks_h.findValues(filtered_hp, middle_pos_hp);
                            }
                        }
                    }


                    vector<double> final_result_values_hp;

                    for(int j = 0; j < middle_peaks_hp.size(); j++)
                    {
                        if(middle_peaks_hp[j] > thr_h)
                        {
                            final_result_values_hp.push_back(middle_peaks_hp[j]);
                        }
                    }
//                    cout << endl << endl;

//       cout << "***GRID LINES values IN HOR.DIRECTION - Final result values of middle peak-elements of horizontal data." << endl;
                    for(int j = 0; j < final_result_values_hp.size(); j++)
                    {
                            static int counter = 0;
//                            cout << ++counter << ": " << final_result_values_hp[j] << "\n";
                    }
//                    cout << endl << endl;

//       cout << "***GRID LINES indices IN HOR.DIRECTION - Final result indices of middle peak-elements of horizontal data." << endl;
                    indices_to_colour_hp = peaks_h.findIndices(filtered_hp, final_result_values_hp);
                    //filtered data is the one appliead on the projections
                    // values are the actual grid peaks
                    //indices_to_colour are the actual indices of the grid i.e. coordinate system
                    for(int j = 0; j < indices_to_colour_hp.size(); j++)
                    {
                            static int counter = 0;
                            //cout << ++counter << ": " << indices_to_colour_hp[j] << "\n";
//                            cout << indices_to_colour_hp[j] << ",";
                    }



Gaussian1DFilter g1f_v;
//filtered_vp = g1f_v.gaussian_filter(vertical_projection0, sigma_v);//input data, sigma


    // Call the function to create a Gaussian kernel
     std::vector<double> kernel2 = g1f_v.createGaussianKernel(sigma_v);
    gaussiankernel2 = kernel2;
    // The  signal that has to be filtered - vertical_projection0
    // Apply the Gaussian filter
    filtered_vp = g1f_v.applyConvolutionWithGaussian1Dfilter(vertical_projection0, kernel2);



QString v_proj_gf;
for (int i = 0; i < filtered_vp.size(); i++)
{
        v_proj_gf +=  QString::number(i) + " | " + QString::number(filtered_vp[i],'f', 6) + '\n';
}
ui->textB_vertical_proj_gaussian->setText(v_proj_gf);
copy_gaussian_v.assign(std::begin(filtered_vp), std::end(filtered_vp));
// VERTICAL PROJECTIONS
        GridlinesDetector peaks_v;
        peaks_v.findPeaks(filtered_vp, filtered_vp_pos, filtered_vp_peaks, 0);
//        cout << "\nPOS: GAUSSIAN on VERTICAL PROJECTIONS" << endl;
 //       peaks_v.print(filtered_vp_pos);
//        cout << "\nPEAKS: GAUSSIAN on VERTICAL PROJECTIONS" << endl;
 //       peaks_v.print(filtered_vp_peaks);
// VERTICAL PROJECTIONS
//        cout << "\n\nFRAME PEAKS: FILTERED VERTICAL PROJECTIONS" << endl;
         // Find left frame peak of vertical projection
         double max_v_frame1 = *max_element(filtered_vp_peaks.begin(), filtered_vp_peaks.end());
//         cout << "max_v_frame1 = " << max_v_frame1 << endl;
         // Remove left frame peak of vertical projection
         vector<double> remove_Vframepeak1;
         remove_Vframepeak1 = filtered_vp_peaks;
         remove_Vframepeak1.erase(std::remove(remove_Vframepeak1.begin(), remove_Vframepeak1.end(), max_v_frame1), remove_Vframepeak1.end());

         // Find right frame peak of horizontal projection
         double max_v_frame2 = *max_element(remove_Vframepeak1.begin(), remove_Vframepeak1.end());
//         cout << "max_v_frame2 = " << max_v_frame2 << endl;
         // Remove right frame peak of horizontal projection
         vector<double> remove_Vframepeak2;
         remove_Vframepeak2 = remove_Vframepeak1; //remove_Vframepeak2 is the one without frame peaks
         remove_Vframepeak2.erase(std::remove(remove_Vframepeak2.begin(), remove_Vframepeak2.end(), max_v_frame2), remove_Vframepeak2.end());

         // Find min value between within the range given by the two frame peaks
         // Needed for the threshold formula
         double min_v_value = *min_element(remove_Vframepeak2.begin(), remove_Vframepeak2.end());
//         cout << "Minimum vertical value: " << min_v_value << endl;
// VERTICAL PROJECTIONS
        // find index of first frame peak INSIDE G.FILTERED VERTICAL VECTOR
        double index_max_v_frame1 = peaks_v.findIndex(filtered_vp, max_v_frame1);
//        cout << "\nIndex of frame peak 1:" << endl;
//        cout << index_max_v_frame1 << endl; // 940

        // find index of second frame peak INSIDE G.FILTERED VECTOR
        double index_max_v_frame2 = peaks_v.findIndex(filtered_vp, max_v_frame2);
//        cout << "\nIndex of frame peak 2:" << endl;
//        cout << index_max_v_frame2 << endl; // 7735

        if(index_max_v_frame2 < index_max_v_frame1)
         {
             double temp = index_max_v_frame2;
             index_max_v_frame2 = index_max_v_frame1;
             index_max_v_frame1 = temp;
//             cout << "\nAfter swap:\nindex_max_v_frame1 = " << index_max_v_frame1 << endl;
//             cout << "index_max_v_frame2 = " << index_max_v_frame2 << endl;
         }
// VERTICAL PROJECTIONS
                    double max_v_pvalue = 0.0;
                    if(max_v_frame1 > max_v_frame2)
                    {
                        max_v_pvalue = max_v_frame1;
                    }
                    else if(max_v_frame1 < max_v_frame2)
                    {
                        max_v_pvalue = max_v_frame2;
                    }
                    double thr_v = (max_v_pvalue - min_v_value)/4.0;
//                    cout << "\nthr_v (max/4) =" << thr_v << endl;

                    for(int i = 0; i < filtered_vp_peaks.size(); i++)
                    {
                        if(index_max_v_frame1 < index_max_v_frame2)
                        {
                            if(filtered_vp_pos[i] > index_max_v_frame1 && filtered_vp_pos[i] < index_max_v_frame2)
                            {
                                middle_pos_vp.push_back(filtered_vp_pos[i]);//indices
                                middle_peaks_vp = peaks_v.findValues(filtered_vp, middle_pos_vp);//values
                            }
                        }
                    }


                    vector<double> final_result_values_vp;
                    for(int j = 0; j < middle_peaks_vp.size(); j++)
                    {
                        if(middle_peaks_vp[j] > thr_v) //use the values of all middle peaks to filter them out using the threshold value
                        {
                            final_result_values_vp.push_back(middle_peaks_vp[j]);
                        }
                    }
//                    cout << endl << endl;

//                   cout << "Final result values of middle peak-elements of vertical data." << endl;
                    for(int j = 0; j < final_result_values_vp.size(); j++)
                    {
                            static int counter = 0;
//                            cout << ++counter << ": " << final_result_values_vp[j] << "\n";

                    }
//                    cout << endl << endl;

//                    cout << "Final result indices of middle peak-elements of vertical data." << endl;
                    indices_to_colour_vp = peaks_v.findIndices(filtered_vp, final_result_values_vp);
                    //filtered data is the one appliead on the projections
                    // values are the actual grid peaks
                    //indices_to_colour are the actual indices of the grid i.e. coordinate system
                    for(int j = 0; j < indices_to_colour_vp.size(); j++)
                    {
                            static int counter = 0;
                            //cout << ++counter << ": " << indices_to_colour_vp[j] << endl;
//                             cout << indices_to_colour_vp[j] << ",";
                    }
//                    cout << endl;





//--------------------------------------------REMOVE UNNECESSARY LINES from the RESEAU LINES RESULTS --------------------------------------------------------
                    GridlinesDetector remove_lines;

//                     QTextStream(stdout) << "\n\nH.PROJ--------REMOVE UNNECESSARY LINES-----" << Qt::endl;

                    if(indices_to_colour_hp.size() > 27 && sigma_h >= 20.0)
                    {
                        filteredNumbers_hp = remove_lines.excludeNeighbours(indices_to_colour_hp);

                        for(int j = 0; j < filteredNumbers_hp.size(); j++)
                        {
                                static int counter = 0;
//                                QTextStream(stdout) << ++counter << ": " << filteredNumbers_hp[j] << Qt::endl;

                        }
//                        cout << endl;

                        indices_to_colour_hp = filteredNumbers_hp;
                     }
                    else
                    {
                        for(int j = 0; j < indices_to_colour_hp.size(); j++)
                        {
                                static int counter = 0;
//                                QTextStream(stdout)  << ++counter << ": " << indices_to_colour_hp[j] << ",";
                        }
//                        cout << endl;
                    }


//                      QTextStream(stdout) << "\n\nV.PROJ--------REMOVE UNNECESSARY LINES-----" << Qt::endl;
                    if(indices_to_colour_vp.size() > 27 && sigma_v >= 20.0)
                    {
                        filteredNumbers_vp = remove_lines.excludeNeighbours(indices_to_colour_vp);


                        for(int j = 0; j < filteredNumbers_vp.size(); j++)
                        {
                                static int counter = 0;
//                                 QTextStream(stdout)  << ++counter << ": " << filteredNumbers_vp[j] << Qt::endl;

                        }
                        cout << endl;

                        indices_to_colour_vp = filteredNumbers_vp;
                    }
                    else
                    {
                        for(int j = 0; j < indices_to_colour_vp.size(); j++)
                        {
                                static int counter = 0;
//                                QTextStream(stdout)  << ++counter << ": " << indices_to_colour_vp[j] << ",";
                        }
                        cout << endl;
                    }




//**********************************************ASTERISMS SEGMENTATION******************************************************************************


//*********************************************************************************************************************************************************************************
                     QImage output_sourceImage0(sourceImage0.size(), QImage::Format_Grayscale8);
                     // Applying Canny
                    // // Convert QImage to cv::Mat
                    // cv::Mat matImage = cv::Mat(sourceImage0.height(), sourceImage0.width(), CV_8UC1, sourceImage0.bits(), sourceImage0.bytesPerLine());
                    // cv::GaussianBlur(matImage, blurredImage, cv::Size(5, 5), 0); // Adjust kernel size as needed
                    // // Step 2: Apply Canny edge detection
                    // cv::Mat edges;
                    // cv::Canny(matImage, edges, 50, 150); // Adjust thresholds as needed
                    // // Step 3: Convert edgesImage back to QImage
                    // QImage outputImage(edges.data, edges.cols, edges.rows, edges.step[0], QImage::Format_Grayscale8);

//**********************************************SOBEL FILTER******************************************************************************

                     SobelFilter sobel;
                     sobel.applySobelEgeDetection(sourceImage0, output_sourceImage0);
                     sobelImage = output_sourceImage0;

//**********************************************ASTERISMS DETECTOR******************************************************************************
                    AsterismsDetector asterismsDetector;

                    vector<double> data;
                    vector<double> rawImageData;
                    // Extract the whole BINARY image data into 1D vector named 'data'
                    for (int i = 0; i < output_sourceImage0.height(); i++)
                    {
                        quint8* ptr_row = (quint8*)(output_sourceImage0.bits() + i * output_sourceImage0.bytesPerLine());
                        for (int j = 0; j < output_sourceImage0.width(); j++)
                        {
                                data.push_back(ptr_row[j]);
                        }
                    }
                    for (int i = 0; i < output_sourceImage0.height(); i++)
                    {
                        quint8* ptr_row = (quint8*)(sourceImage0.bits() + i * sourceImage0.bytesPerLine());
                        for (int j = 0; j < sourceImage0.width(); j++)
                        {
                                rawImageData.push_back(ptr_row[j]);
                        }
                    }

                    int imageWidth = output_sourceImage0.width();
                    int imageHeight = output_sourceImage0.height();


                    vector<double> Row =  indices_to_colour_vp; // Vertical projections
                    vector<double> Col =  indices_to_colour_hp; // Horizontal projections


                    // extractSubSquares - store each sub-square's data independently for later access
                    std::vector<std::vector<double>> subSquareDataCollection;


                    // Parameters used in Opencv Hough Circle Transform
                    double dp = 1;          // Inverse ratio of the accumulator resolution to the image resolution.
                    double minDist = 6.0;   // Minimum distance between the centers of detected circles.
                    double param1 = 110.0;  // Accumulator threshold for circle detection. It helps control the sensitivity of the circle detection.
                    double param2 = 4.0;    // Accumulator threshold for circle detection
                    double minRadius = 3;   // Minimum radius of detected circles
                    double maxRadius = 6;   // Maximum radius of detected circles



                   asterismsDetector.detectAsterismsFromEachReseauSubSquareRegion(Col, Row, rawImageData, data,imageHeight, imageWidth, subSquareDataCollection, dp,  minDist,  param1,  param2,  minRadius,  maxRadius, plateName);
/*
 ************** Testing with Astrographic Map Image - ROB033_000008.tif  **************

        Final result indices of middle peak-elements of horizontal data (27 reseau lines), provideing the Y coordinates
            1366,1609,1848,2086,2325,2565,2803,3043,3283,3522,3761,4001,4239,4479,4719,4959,5198,5437,5678,5916,6156,6398,6640,6879,7116,7355,7600,
        Final result indices of middle peak-elements of vertical data (27 reseau lines),  providing the X coordinates
            1130,1371,1611,1851,2092,2331,2571,2811,3052,3291,3531,3770,4011,4251,4491,4731,4972,5212,5451,5691,5931,6172,6413,6653,6893,7134,7376

         We get the following pair - x and y, of coordinates marked with '*',
         the top-left corner and the bottom-rigth corner of a given sub-image or square:

                 (sVerticalProj,sHorizontalProj)
                         * __ __ ___
                         |          |
                         |          |
                         |          |
                          ___ __ __ |
                                    *  (eVerticalProj,eHorizontalProj)


        For example, having:
                 vector<double> HorizontalProjections =  {1366,1609}; // 1188,1431};
                 vector<double> Verticalprojections = {1130,1371}; //1285,1525};

        This means:

                 (1130,1366)
                         * __ __ ___
                         |          |
                         |          |
                         |          |
                          ___ __ __ |
                                    *  (1371,1609)


        Dimensions calculations:
            HorizontalProjections calculations: 1366 - 1130 =  236
            Verticalprojections calculations: 1609 - 1371 =  238
                So, 236x238 = 56168 pixel-values ONLY within this specific sub-square, this is actually the very first sub-square region in the reseau!
                This last pixel-value within this sub-squareregion stays at index: (1371-1, 1609-1) or (1370, 1608)

                           (sVerticalProj, sHorizontalProj) - (eVerticalProj, eHorizontalProj)
                So, this means that our first sub-square region in the reseau stays at indices: (1130,1366)  -  (1371,1609),
                the very fist place where both horizontal (blue) and vertical (red) lines intersect each other.

                NOTE: This could be seen on the GUI, when the button 'Show Grid lines on the Astrographic map' is pressed,
                      within the code it is provided by on_pushB_showCoord_clicked().
*/

        // Display
        item = new QGraphicsPixmapItem(QPixmap::fromImage(this->sourceImage));
        scene = new QGraphicsScene(this);
        ui->graphics_view1->setScene(scene);
        scene->addItem(item);

        sourceImage1 = sourceImage;
    }
    else
    {
        QMessageBox::critical(this,"Error","Error");
        qDebug("Error!");
    }

}// on_pushB_applyAlgorithms_clicked





// -------------------------------------------------------- Charts of Histogram, Projections and Derivatives results + Painter of the segmented sub-square regions of the reseau + some indices of the first region added --------------------

/**
 * Histogram Chart
 */
void CarteDuCiel::on_showChart_histo_clicked()
{
   //Sources: https://datacarpentry.org/image-processing/07-thresholding/
   hchart = new HistogramChart(this);

   if (copy_histo.size() <= 0)
    {
       qDebug() << "Histogram Data empty!";
       QMessageBox::information(this, "Important information", "Histogram Data empty!", QMessageBox::Ok);

    }
    else
    {
        hchart->setData(copy_histo, cumulative_sum, th, between_class_variance);
        hchart->setData2(copy_histo, cumulative_sum, th, between_class_variance);
        hchart->show();
    }
}// on_showChart_histo_clicked

/**
 * Projections and Derivatives Charts along Vertical direction
 *      Also, the setData_gaussian1dfilter has a modified function that could be used for drawing a Gaussian 1D kernel on a graph,
 *      providing the bell-curve visualization, see file chart_projections.cpp for more details and uncomment the version of the setData_gaussian1dfilter
 *      you want to use.
*/
void CarteDuCiel::on_showChart_vr_projections_clicked()
{
        verchart = new DerivativesChart(this);

        if (vertical_projection0.size() <= 0)
         {
            qDebug() << "No projections calculated!";
            QMessageBox::information(this, "Important information", "Still no projections calculated!", QMessageBox::Ok);
         }
         else
         {
            verchart->setData_scanline(vertical_projection0); // Vertical Projections i.e. values of scanline
//          verchart->setData_gaussian1dfilter(gaussiankernel2); // Gaussian kernel could be printed instead
            verchart->setData_gaussian1dfilter(copy_gaussian_v);
            verchart->setData_difference(vertical_projection0,copy_gaussian_v);

            verchart->setData_firstDerivative(copy_first_derivative_v);
            verchart->setData_secondDerivative(copy_second_derivative_v);

            verchart->show();

         }
}// on_showChart_vr_projections_clicked

/**
 * Projections and Derivatives Charts along Horizontal direction
 *      Also, the setData_gaussian1dfilter has a modified function that could be used for drawing a Gaussian 1D kernel on a graph,
 *      providing the bell-curve visualization, see file chart_projections.cpp for more details and uncomment the version of the setData_gaussian1dfilter
 *      you want to use.
*/
void CarteDuCiel::on_showChart_hr_projections_clicked()
{
        horchart = new DerivativesChart(this);

        if (horizontal_projection0.size() <= 0)
         {
            qDebug() << "No projections calculated!";
            QMessageBox::information(this, "Important information", "Still no projections calculated!", QMessageBox::Ok);
         }
         else
         {
            horchart->setData_scanline(horizontal_projection0); // Horizontal Projections i.e. values of scanline
//          horchart->setData_gaussian1dfilter(gaussiankernel1); // Gaussian kernel could be printed instead
            horchart->setData_gaussian1dfilter(copy_gaussian_h);

            horchart->setData_difference(horizontal_projection0, copy_gaussian_h);
            horchart->setData_firstDerivative(copy_first_derivative_h);
            horchart->setData_secondDerivative(copy_second_derivative_h);

            horchart->show();
         }
}// on_showChart_hr_projections_clicked

/** 
 * Draw the horizontal and vertical lines of the projective profiles on the processed image 
 * to visualize the segmented grid result better.
*/
void CarteDuCiel::on_pushB_showCoord_clicked()
{
        sourceImage1 = sourceImage1.convertToFormat(QImage::Format_ARGB32);

        QPainter painter(&sourceImage1);
        QFont font = painter.font();
        font.setPointSize(4);
        painter.setFont(font);

        // Draw Horizontal lines        
        painter.setPen( QPen(QColor(48, 186, 216, 255), 5)); // Light Blue - horizontal lines
        vector<double> y_coords = indices_to_colour_hp; // filteredNumbers_hp;
        for (int y : y_coords)
        {
            for (int x = 0; x < sourceImage1.width(); x++)
            {
                if (x >= 0 && x < sourceImage1.width() && y >= 0 && y < sourceImage1.height())
                {
                    painter.drawPoint(x, y);
                }
            }
        }
        // Draw Vertical lines
        painter.setPen(QPen(QColor(230, 58, 45, 255), 5)); // Light red - vertical projections
        vector<double> x_coords =  indices_to_colour_vp; // filteredNumbers_vp;
        for (int x : x_coords)
        {
            for (int y = 0; y < sourceImage1.height(); y++)
            {
                if (x >= 0 && x < sourceImage1.width() && y >= 0 && y < sourceImage1.height())
                {
                    painter.drawPoint(x, y);
                }
            }
        }

        /** 
         * Add allcoordinates of top-left and bottom-right corners of some of the segmented sub-square within the reseau,
         * where the horizontal (Y index) and vertical (X index) projections intersect each other.
         */ 
        for (size_t i = 0; i < x_coords.size(); ++i)
        {
            int x = x_coords[i];
            int y = y_coords[i];
            painter.setPen(QPen(QColor(62, 62, 62, 255), 5));
            if (x >= 0 && x < 1372 && y >= 0 && y < 1610) // Specific setting for plate ROB033/08 only for the very first sub-square segmented region of the reseau
//            if (x >= 0 && x < sourceImage1.width() && y >= 0 && y < sourceImage1.height()) // Just one diagonal with applied labels of the generated indices from the provided projection calculations of some of the segmented region but not all (just for an example visualization)
            {
                // Draw the point
                painter.drawPoint(x, y);
                // Draw the y-coordinate label
                painter.drawText(x + 15, y - 10, QString("x:%1").arg(x)); // Above the point
                painter.drawText(x + 15, y + 45, QString("y:%1").arg(y)); // Below the point
            }
            else {
                qWarning() << "Point out of bounds:" << x << y;
            }
        }

        // Finalize drawing and display the image
        painter.end();

        // Create your CoordinatesInspector widget
       imageViewer = new CoordinatesInspector(ui->graphics_view2, this->sourceImage1);
       imageViewer->show();
}//on_pushB_showCoord_clicked

