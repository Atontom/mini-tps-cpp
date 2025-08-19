/*!
 *\file bloc3d.cpp
 *\brief Fichier source de la classe Bloc 3D
 * \version 0.0
 * \date 18/04/2017
 * \author Xavier Franceries, Pierre Freton, Hubert Caquineau
 * \ brief classe liée à l'interface graphique par son membre "ui"
 * Copyright UE7 - Optimisation
 *****************************************************************************/

#ifndef FENETREBASE_H
#define FENETREBASE_H


//------------------------ INCLUDES
// INCLUDES Systeme / Déclaration de classes


// INCLUDES Standard Librairie / Déclaration de classes
#include <iostream>

// INCLUDES QT / Déclaration de classes
#include <QMainWindow>
#include <QLabel>
#include <QSlider>
#include "clickablelabel.h"


// Autres INCLUDES / Déclaration de classes
extern "C"
{

}

// INCLUDES propres au TP/ Déclaration de classes
#include "bloc3d.h"

using namespace std;

namespace Ui {
class FenetreBase;
}

class FenetreBase : public QMainWindow
{
    Q_OBJECT

//----------------------------------------------------------------------Methodes publiques
public:
    explicit FenetreBase(QWidget *parent = 0);
    ~FenetreBase();

    QImage getQImage();



//----------------------------------------------------------------------SLOTS
private slots:
    void on_Test();
    void on_Close();

    void on_SliderBouge();
    void on_SliderBougeCor();
    void on_SliderBougeSag();
    void on_SliderBougeDose();
    void on_SliderBougeDensity();

    void precalculerKERMA();

    void onAxialMouseMoved(int x, int y);
    void onSagittalMouseMoved(int y, int z);
    void onCoronalMouseMoved(int x, int z);
    void syncSliders(int value);

    int afficheToSliceZ(int pixelY, int totalSlices);



public slots :
    void on_ComboBox_Changed(int index);
    void onImageMouseMoved(int x, int y);

    void updateAllImages();
    int screenZToSliceZ(int screenZ);





//----------------------------------------------------------------------MEMBRES
private:
    Ui::FenetreBase *ui;

    QImage m_Image;
    QImage m_ImageCor;
    QImage m_ImageSag;

    enum ModeAffichage{DoseGradient = 0, DoseIso = 1, DRR = 2, Densite = 3, PseudoCouleur = 4};

    QImage m_TermaMap;
    QImage m_Density;
    QImage m_DRR;
    QImage m_PseudoCouleur;

    bloc3d m_Bloc3D;     // Attention toute variable membre est crée dès l'instanciation de la classe FenetreBase

    QLabel m_ImageLabel; //Pour afficher une QImage après
    QLabel m_ImageLabelCor;
    QLabel m_ImageLabelSag;

    QLabel m_TermaMapLabel;
    QLabel m_DensityLabel;
    QLabel m_DRRLabel;
    QLabel m_PseudoCouleurLabel;


    bool syncing = false;


   // std::vector<std::vector<QImage>> KERMAs;

    std::vector<QImage> kermaSlicesGrad;
    std::vector<QImage> kermaSlicesIso;

    /*std::vector<QImage> kermaSlices2;
    std::vector<QImage> kermaSlices3;
    std::vector<QImage> kermaSlices4;
    std::vector<QImage> kermaSlices5;
*/
};

#endif // FENETREBASE_H
