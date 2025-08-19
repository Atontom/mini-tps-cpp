/*!
 *\file FenetreBase.cpp
 *\brief Fichier source de la classe FenetreBase
 * \version 0.0
 * \date 27/03/2018
 * \author Xavier Franceries, Pierre Freton, Hubert Caquineau
 * \version 0.1 24/04/2018 XF Modification in constructor with setparent for Qimage
 * \ brief classe liée à l'interface graphique par son membre "ui"
 * Copyright RMGBM
 *****************************************************************************/

#include "fenetrebase.h"
#include "clickablelabel.h"
#include "ui_fenetrebase.h"
#include<algorithm>
#include<QDebug>

template <typename T>
T clamp(T val, T minVal, T maxVal) {
    return std::max(minVal, std::min(maxVal, val));
}


//----------------------------------------------------------------------CONSTRUCTEUR
FenetreBase::FenetreBase(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FenetreBase)
{
    ui->setupUi(this);


    //Mvt Souris
    QObject::connect(ui->widgetImage_Dose, &ClickableLabel::mouseMoved, this, &FenetreBase::onImageMouseMoved);

    //curseurs
    QObject::connect(qobject_cast<ClickableLabel*>(ui->widgetImage),      &ClickableLabel::mouseMoved, this, &FenetreBase::onAxialMouseMoved);
    QObject::connect(qobject_cast<ClickableLabel*>(ui->widgetImage_Cor),  &ClickableLabel::mouseMoved, this, &FenetreBase::onCoronalMouseMoved);
    QObject::connect(qobject_cast<ClickableLabel*>(ui->widgetImage_Sag),  &ClickableLabel::mouseMoved, this, &FenetreBase::onSagittalMouseMoved);



    // Push button
    QObject::connect(ui->pushButtonFaire, SIGNAL (clicked()), this, SLOT (on_Test()));
    QObject::connect(ui->pushButtonQuitter, SIGNAL (clicked()), this, SLOT (on_Close()));

    // Vertical Slider
    QObject::connect(ui->verticalSliderSelect, SIGNAL (valueChanged(int)), this, SLOT (on_SliderBouge()));
    QObject::connect(ui->verticalSliderSelect_Sag, SIGNAL (valueChanged(int)), this, SLOT (on_SliderBougeSag()));
    QObject::connect(ui->verticalSliderSelect_Cor, SIGNAL (valueChanged(int)), this, SLOT (on_SliderBougeCor()));
    QObject::connect(ui->verticalSliderSelect_Dose, SIGNAL (valueChanged(int)), this, SLOT (on_SliderBougeDose()));


    // synchronisation des sliders
    QObject::connect(ui->verticalSliderSelect, &QSlider::valueChanged, this, &FenetreBase::syncSliders);
    QObject::connect(ui->verticalSliderSelect_Dose, &QSlider::valueChanged, this, &FenetreBase::syncSliders);
    QObject::connect(ui->verticalSliderSelect_Cor, &QSlider::valueChanged, this, &FenetreBase::syncSliders);
    QObject::connect(ui->verticalSliderSelect_Sag, &QSlider::valueChanged, this, &FenetreBase::syncSliders);

    // Combo Box
    QObject::connect(ui->comboBox, SIGNAL (currentIndexChanged(int)), this, SLOT (on_ComboBox_Changed(int)));


    string prefix = "/Users/etudiantM1/Desktop/Pack_EtudiantM1_2020/ProjetM1/Patient1OrdonneASCII/Patient1.";
    m_Bloc3D.changeBlocSize(2);

    cout << "1-Adresse m_Bloc3D = %h " << &m_Bloc3D << endl;

    m_Bloc3D.loadASCII( prefix );// CHARGEMENT DU BLOC 3D (images médicales en coupe du CT OU de l'IRM ou autre)


    cout<<"Début du Calcul du KERMA ..."<<endl;
    precalculerKERMA();
    // m_Bloc3D.PrintDoseSlice(10);
    cout<<"KERMA Calculé ! "<<endl;

/*
    connect(ui->verticalSliderSelect_Cor, &QSlider::valueChanged, ui->verticalSliderSelect_Sag, &QSlider::setValue); // synchronisation coronal -> sag
    connect(ui->verticalSliderSelect_Sag, &QSlider::valueChanged, ui->verticalSliderSelect_Cor, &QSlider::setValue); // synchronisation Sag -> Coronal



    connect(ui->verticalSliderSelect_Dose, &QSlider::valueChanged, ui->verticalSliderSelect, &QSlider::setValue); // synchronisation axial -> Dose
    connect(ui->verticalSliderSelect, &QSlider::valueChanged, ui->verticalSliderSelect_Dose, &QSlider::setValue); // synchronisation Dose -> axial
*/

    /*    Rappel pour afficher une image :
    //-----------------------------------------------
      m_ImageLabel.setParent(ui->widgetImage);
      bool l_test = m_Image.load("/Users/etudiantM1/Desktop/Projet M1/imageTest.png");
      if (l_test)
      {
        m_ImageLabel.setPixmap(QPixmap::fromImage(m_Image));
        cout << "FenetreBase::FenetreBase CHARGEMENT IMAGE REUSSI ! " << endl;
      }
      else
       cout << "FenetreBase::FenetreBase ERREUR------- FICHIER IMAGE INTROUVABLE !!!!!! VERIFIER CHEMIN" << endl;
     */





    /*  Rappel pour récupérer une valeur d'un pixel par exemple en NG :
    //-----------------------------------------------------------------
      int l_ng = qGray(m_Image.pixel(10,10)); //récupération en NG (0 à 256) de la valeur du pixel en (5,5)
      cout << " La VALEUR DU PIXEL(5,5) en niveau de gris de l'image est : " << l_ng << endl;
    */

    //A faire obligatoirement dans le constructeur :
    m_ImageLabel.setParent(ui->widgetImage);//Sinon ne fonctionne pas apres pour l'affichage par les methodes ulterieures
    //m_ImageLabelCor.setParent(ui->widgetImage_Cor);
    //m_ImageLabelSag.setParent(ui->widgetImage_Sag);
    //m_TermaMapLabel.setParent(ui->widgetImage_Dose);

    bool l_test = m_Image.load("/Users/etudiantM1/Desktop/Pack_EtudiantM1_2020/ProjetM1/Lenna.png");
    if (l_test)
    {
      m_ImageLabel.setPixmap(QPixmap::fromImage(m_Image));
      cout << "FenetreBase::FenetreBase CHARGEMENT IMAGE REUSSI ! " << endl;
    }
    else
     cout << "FenetreBase::FenetreBase ERREUR------- FICHIER IMAGE INTROUVABLE !!!!!! VERIFIER CHEMIN" << endl;

     ui->verticalSliderSelect->setSliderPosition(1);
     m_ImageLabel.hide();


     m_ImageLabel.setParent(ui->widgetImage);
     m_Image = m_Bloc3D.qimageFromCoupe(50);//coupe 5 pour test (attention il faudra tester et prévoir s'il y a une erreur e numéro de coupe existant pas...)
     m_ImageLabel.setPixmap(QPixmap::fromImage(m_Image));

     m_ImageLabelCor.setParent(ui->widgetImage_Cor);
     m_ImageCor = m_Bloc3D.GetCorView(50);
     m_ImageLabelCor.setPixmap(QPixmap::fromImage(m_ImageCor));

     m_ImageLabelSag.setParent(ui->widgetImage_Sag);
     m_ImageSag = m_Bloc3D.GetSagView(50);
     m_ImageLabelSag.setPixmap(QPixmap::fromImage(m_ImageSag));


     m_TermaMapLabel.setParent(ui->widgetImage_Dose);
     m_TermaMap = kermaSlicesGrad[50];
     m_TermaMapLabel.setPixmap(QPixmap::fromImage(m_TermaMap));

}

//----------------------------------------------------------------------Methodes publiques
QImage FenetreBase::getQImage()
{
    return m_Image;
}

//Image.save("/Users/etudiantM2/Desktop/TP3/lena3.png");


//----------------------------------------------------------------------SLOTS

void FenetreBase::precalculerKERMA(){
    cout<<"Commencement du precalcul du KERMA..."<< endl;
    kermaSlicesGrad.clear();
    kermaSlicesIso.clear();
    int NbSlices = 191;
    for (int z = 0; z < NbSlices; z++)
    {
        std::pair<QImage, QImage> result = m_Bloc3D.GetKermaMap(z, 0.1 , 3e9, 400, 120);
        kermaSlicesGrad.push_back(result.first);
        kermaSlicesIso.push_back(result.second);

        cout<<"KERMA calculé pour la couche : "<<z<<endl;
    }
}

void FenetreBase::on_Test()
{

  cout << " Les images ont été chargées " << endl;

  cout<<"Début du calcul du KERMA pour le volume ..."<<endl;

  cout<<"KERMA Caculé !"<<endl;
  /*
   //m_bloc3D = bloc3d(5);
   cout << "3-Adresse m_Bloc3D = %h " << &m_Bloc3D << endl;

   if (m_Bloc3D.getSizeZ() != 0)
   {
     cout << " recuperation de la valeur du bloc en 5,5,5 =" << m_Bloc3D.getValue(1,1,1) << endl;
   }
   else
   {
     cout << " --- erreur bloc3D vide ---- !!!!!" << endl;
   }
   */




}


void FenetreBase::on_Close()
{
    close();
}

//----------------------------------------------------------------------SLOTS
void FenetreBase::on_SliderBouge()
{

  cout << " ------FenetreBase::on_SliderBouge()--------- " << endl;

  int l_val = ui->verticalSliderSelect->value();

  cout << " ------FenetreBase::on_SliderBouge()--------- l_val =" << l_val << "....." << endl;

  //m_ImageLabel.setParent(ui->widgetImage);
  m_Image = m_Bloc3D.qimageFromCoupe(l_val); //coupe 5 pour test (attention il faudra tester et prévoir s'il y a une erreur e numéro de coupe existant pas...)
  m_ImageLabel.setPixmap(QPixmap::fromImage(m_Image));

  m_ImageLabel.show();
  cout << " ------FenetreBase::on_SliderBouge()---------FIN " << endl;
/*
  cout << " ..... " << endl;
  int w = m_Image.width();
  int h = m_Image.height();
  cout << " Taille image de taille : " << w << " x " << h << " ... " << endl;
*/
}


void FenetreBase::on_SliderBougeSag(){

    cout << " ------FenetreBase::on_SliderBouge()--------- " << endl;

    int l_val = ui->verticalSliderSelect_Sag->value();

    cout << " ------FenetreBase::on_SliderBougeSag()--------- l_val =" << l_val << "....." << endl;

    m_ImageLabelSag.setParent(ui->widgetImage_Sag);
    m_ImageSag = m_Bloc3D.GetSagView(l_val); //coupe 5 pour test (attention il faudra tester et prévoir s'il y a une erreur e numéro de coupe existant pas...)
    m_ImageLabelSag.setPixmap(QPixmap::fromImage(m_ImageSag));
    m_ImageLabelSag.show();
    cout << " ------FenetreBase::on_SliderBougeSag()---------FIN " << endl;
  /*
    cout << " ..... " << endl;
    int w = m_Image.width();
    int h = m_Image.height();
    cout << " Taille image de taille : " << w << " x " << h << " ... " << endl;
*/
}


void FenetreBase::on_SliderBougeCor(){

    cout << " ------FenetreBase::on_SliderBouge()--------- " << endl;

    int l_val = ui->verticalSliderSelect_Cor->value();

    cout << " ------FenetreBase::on_SliderBougeCor()--------- l_val =" << l_val << "....." << endl;

    m_ImageLabelCor.setParent(ui->widgetImage_Cor);
    m_ImageCor = m_Bloc3D.GetCorView(l_val); //coupe 5 pour test (attention il faudra tester et prévoir s'il y a une erreur de numéro de coupe existant pas...)
    m_ImageLabelCor.setPixmap(QPixmap::fromImage(m_ImageCor));
    m_ImageLabelCor.show();

    cout << " ------FenetreBase::on_SliderBougeCor()---------FIN " << endl;

    /*
    cout << " ..... " << endl;
    int w = m_Image.width();
    int h = m_Image.height();
    cout << " Taille image de taille : " << w << " x " << h << " ... " << endl;
    */

}
/*
void FenetreBase::on_SliderBougeDose()
{
    int Dose_slider = ui ->verticalSliderSelect_Dose->value();



    m_TermaMap = kermaSlices[Dose_slider];
    m_TermaMapLabel.setPixmap(QPixmap::fromImage(m_TermaMap));
    m_TermaMapLabel.show();

/*
    m_TermaMap = m_Bloc3D.GetKermaMap(Dose_slider, 0.1 , 1e6, 512, 90);
    m_TermaMapLabel.setPixmap(QPixmap::fromImage(m_TermaMap));
    m_TermaMapLabel.show();


    m_Density = m_Bloc3D.DensityVol(Dose_slider);
    m_DensityLabel.setPixmap(QPixmap::fromImage(m_Density));
    m_DensityLabel.show();

}*/
void FenetreBase::on_SliderBougeDose()
{
    on_ComboBox_Changed(ui->comboBox->currentIndex());

}
void FenetreBase::on_SliderBougeDensity()
{
    int Dose_slider = ui ->verticalSliderSelect_Dose->value();
    m_Density = m_Bloc3D.DensityVol(Dose_slider);
    m_DensityLabel.setPixmap(QPixmap::fromImage(m_Density));
    m_DensityLabel.show();
}

void FenetreBase::on_ComboBox_Changed(int index)
{
    int z = ui->verticalSliderSelect_Dose->value();

    m_TermaMapLabel.hide();
    m_DensityLabel.hide();
    m_DRRLabel.hide();
    m_PseudoCouleurLabel.hide();


    if (index == DoseGradient)
    {   cout<<"Mode Dose"<<endl;
        ui->verticalSliderSelect_Dose->setEnabled(true);
        m_TermaMapLabel.setParent(ui->widgetImage_Dose);
        m_TermaMap = kermaSlicesGrad[z];
        m_TermaMapLabel.setPixmap(QPixmap::fromImage(m_TermaMap));
        m_TermaMapLabel.show();

    }
    else if (index == DoseIso)
    {   cout<<"Mode Dose"<<endl;
        ui->verticalSliderSelect_Dose->setEnabled(true);
        m_TermaMapLabel.setParent(ui->widgetImage_Dose);
        m_TermaMap = kermaSlicesIso[z];
        m_TermaMapLabel.setPixmap(QPixmap::fromImage(m_TermaMap));
        m_TermaMapLabel.show();

    }
    else if (index == Densite)
    {   cout<<"Mode Densité"<<endl;
        ui->verticalSliderSelect_Dose->setEnabled(true);
        m_DensityLabel.setParent(ui->widgetImage_Dose);
        m_Density = m_Bloc3D.DensityVol(z);
        m_DensityLabel.setPixmap(QPixmap::fromImage(m_Density));
        m_DensityLabel.show();

    }
   else if (index == DRR)
    {   cout<<"Mode DRR"<<endl;
        ui->verticalSliderSelect_Dose->setEnabled(false);
        m_DRRLabel.setParent(ui->widgetImage_Dose);
        m_DRR = m_Bloc3D.GetDRRView();
        m_DRRLabel.setPixmap(QPixmap::fromImage(m_DRR));
        m_DRRLabel.show();
        disconnect(ui->verticalSliderSelect_Dose, &QSlider::valueChanged, ui->verticalSliderSelect, &QSlider::setValue); // synchronisation axial -> Dose
        disconnect(ui->verticalSliderSelect, &QSlider::valueChanged, ui->verticalSliderSelect_Dose, &QSlider::setValue); // synchronisation Dose -> axial

    }
    else if (index == PseudoCouleur)
    {   cout<<"Mode Pseudo Couleur"<<endl;
        ui->verticalSliderSelect_Dose->setEnabled(true);
        m_PseudoCouleurLabel.setParent(ui->widgetImage_Dose);
        m_PseudoCouleur = m_Bloc3D.PseudoCouleur(z);
        m_PseudoCouleurLabel.setPixmap(QPixmap::fromImage(m_PseudoCouleur));
        m_PseudoCouleurLabel.show();

    }

}

void FenetreBase::onImageMouseMoved(int x, int y)
{
    int z = ui->verticalSliderSelect_Dose->value();
    float doseGy = m_Bloc3D.GetDoseAt(x,y,z);
    QString text = QString("x = %1, y = %2, z = %3 - Dose : %4 Gy")
            .arg(x)
            .arg(y)
            .arg(z)
            .arg(doseGy, 0, 'f', 2 );
    ui->LabelDose->setText(text);
}



void FenetreBase::updateAllImages()
{
    int i = m_Bloc3D.getCursorI();
    int j = m_Bloc3D.getCursorJ();
    int k = m_Bloc3D.getCursorK();

    qDebug() << "[Update] i=" << i << " j=" << j << " k=" << k;

    m_Image = m_Bloc3D.qimageFromCoupe(k);
    qDebug() << "Axiale image loaded";
    m_ImageLabel.setParent(ui->widgetImage);  // <- ajoute cette ligne
    m_ImageLabel.setPixmap(QPixmap::fromImage(m_Image));

    m_ImageCor = m_Bloc3D.GetCorView(j);
    qDebug() << "Coronale image loaded";
    m_ImageLabelCor.setParent(ui->widgetImage_Cor);
    m_ImageLabelCor.setPixmap(QPixmap::fromImage(m_ImageCor));

    m_ImageSag = m_Bloc3D.GetSagView(i);
    qDebug() << "Sagittale image loaded";
    m_ImageLabelSag.setParent(ui->widgetImage_Sag);
    m_ImageLabelSag.setPixmap(QPixmap::fromImage(m_ImageSag));
}







void FenetreBase::onAxialMouseMoved(int x, int y)
{
    if (x < 0 || x >= 512 || y < 0 || y >= 512)
        return;

    int z = ui->verticalSliderSelect->value();

    m_Bloc3D.SetCursor(x, y, z);

    ui->verticalSliderSelect_Sag->setValue(x);
    ui->verticalSliderSelect_Cor->setValue(y);

    updateAllImages();
}

int FenetreBase::screenZToSliceZ(int screenZ) {
    // Remet à l'échelle l'axe z affiché (512px) vers axe z du volume (191 slices)
    float factor = 191.0f / 512.0f;
    int z = static_cast<int>(screenZ * factor);
    return clamp(z, 0, 190); // pour éviter les dépassements
}

void FenetreBase::onSagittalMouseMoved(int x, int z_affiche)
{
    int y = x;
    int z = screenZToSliceZ(z_affiche);
    int x_slider = ui->verticalSliderSelect_Sag->value();

    if (y < 0 || y >= 512 || z < 0 || z >= 191)
        return;

    m_Bloc3D.SetCursor(x_slider, y, z);

    ui->verticalSliderSelect_Cor->setValue(y);
    ui->verticalSliderSelect->setValue(z);

    updateAllImages();
}

void FenetreBase::onCoronalMouseMoved(int x, int z_affiche)
{
    int z = screenZToSliceZ(z_affiche);
    int y_slider = ui->verticalSliderSelect_Cor->value();

    if (x < 0 || x >= 512 || z < 0 || z >= 191)
        return;

    m_Bloc3D.SetCursor(x, y_slider, z);

     ui->verticalSliderSelect_Sag->setValue(x);
    ui->verticalSliderSelect->setValue(z);

    updateAllImages();
}





void FenetreBase::syncSliders(int value)
{
    if (syncing)
        return;

    syncing = true;

    QString mode = ui->comboBox_2->currentText();
    QSlider* senderSlider = qobject_cast<QSlider*>(sender());

    if (mode == "SYNC AX_DOSE ON") {
        if (senderSlider == ui->verticalSliderSelect)
            ui->verticalSliderSelect_Dose->setValue(value);
        else if (senderSlider == ui->verticalSliderSelect_Dose)
            ui->verticalSliderSelect->setValue(value);

    } else if (mode == "SYNC COR_SAG ON") {
        if (senderSlider == ui->verticalSliderSelect_Cor)
            ui->verticalSliderSelect_Sag->setValue(value);
        else if (senderSlider == ui->verticalSliderSelect_Sag)
            ui->verticalSliderSelect_Cor->setValue(value);

    } else if (mode == "SYNC ON (ALL)") {
        // Synchro 1 & 2
        if (senderSlider == ui->verticalSliderSelect)
            ui->verticalSliderSelect_Dose->setValue(value);
        else if (senderSlider == ui->verticalSliderSelect_Dose)
            ui->verticalSliderSelect->setValue(value);
        // Synchro 3 & 4
        else if (senderSlider == ui->verticalSliderSelect_Cor)
            ui->verticalSliderSelect_Sag->setValue(value);
        else if (senderSlider == ui->verticalSliderSelect_Sag)
            ui->verticalSliderSelect_Cor->setValue(value);
    }

    // mode SYNC OFF => ne rien faire

    syncing = false;
}



int FenetreBase::afficheToSliceZ(int pixelY, int totalSlices) {
    return clamp(int(pixelY * (totalSlices / 512.0f)), 0, totalSlices - 1);
}
//----------------------------------------------------------------------DESTRUCTEUR
FenetreBase::~FenetreBase()
{
    delete ui;
}
