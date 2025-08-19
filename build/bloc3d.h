/*!
 *\file bloc3d.h
 *\brief Fichier source de la classe Bloc 3D
 * \version 0.0
 * \date 27/03/2018
 * \author Xavier Franceries, Pierre Freton, Hubert Caquineau
 * \ brief permet d'avoir un squelette pour une classe de base
 * Copyright RMGBM
 *****************************************************************************/

#ifndef BLOC3D_H
#define BLOC3D_H


//------------------------ INCLUDES
// INCLUDES Systeme / Déclaration de classes


// INCLUDES Standard Librairie / Déclaration de classes
#include<vector>
#include <iostream>

// INCLUDES QT / Déclaration de classes
#include <QImage>

// INCLUDES VTK  / Déclaration de classes
//#include <vtkSmartPointer.h>
//#include <vtkImageData.h>
//#include <vtkMatrix4x4.h>
//#include <vtkStringArray.h>

// Autres INCLUDES / Déclaration de classes
extern "C"
{

}

// INCLUDES propres au TP/ Déclaration de classes

using namespace std;

/*!
 *\brief Classe Gerant un bloc de donnees 3D (e.g. un bloc de voxels IRM)
 */

  class bloc3d
   {

    public:
      //------------------------ Constantes publiques
    std::vector<std::vector<std::vector<double>>> m_PhysicalKerma;
    int m_cursor_i = 0;
    int m_cursor_j = 0;
    int m_cursor_k = 0;

      //------------------------ Constructeur destructeur publiques

 /*!
  *\fn  bloc3d
  *\brief constructeur de la classe (vide)
  */
      bloc3d();

  /*!
   *\fn  bloc3d
   *\brief constructeur de la classe : Bloc3D de taille a_size (rempli de zero)
   */
      bloc3d(int a_size);

 /*!
  *\fn  ~bloc3d
  *\brief destructeur de la classe
  */
      virtual ~bloc3d();

//----------------------------------

       /*!
       *\fn  loadDicom
       *\brief methode de chargement d'un bloc 3D DICOM avec bool=1 si reussi
       *
       */
      virtual bool loadDicom();

      /*!
       *\fn  loadDicom
       *\brief methode de chargement d'un bloc 3D DICOM avec bool=1 si reussi
       *
       */
       virtual void loadASCII(string a_prefix);

      /*!
       *\fn  getValue
       *\brief recuperation de la valeur en X,Y,Z du bloc3D
       */
      double getValue(int a_x, int a_y, int a_z) {cout<<"   1 -" << endl; double toto = m_Bloc3D[a_x][a_y][a_z]; return (toto);}

      /*!
       *\fn  getSize
       *\brief recuperation de la valeur en X,Y du bloc3D
       */
      int getSizeXY() {cout << "    size XY = "<<endl;return m_SizeXY;}

      /*!
       *\fn  getSize
       *\brief recuperation de la valeur en Z du bloc3D
       */
      int getSizeZ() {cout << "    size Z = "<<endl;return m_SizeZ;}

      /*!
       *\fn  changeBlocSize
       *\brief modification de la taille du bloc
       */
      void changeBlocSize(int a_size) {init(a_size);}


      float GetDoseAt(int x, int y, int z) const ;


      /*!
       *\fn  GetSagView()
       *\brief Séléctionne la coupe Sagittale a la profondeur int X
       */

      QImage GetSagView(int x);


      /*!
       *\fn  GetCorView()
       *\brief Séléctionne la coupe Coronale a la profondeur int y
       */
      QImage GetCorView(int y);


      /*!
       *\fn  GetKermaMap()
       *\brief
       * Input : int z ->  la coupe du volume a la profondeur z, float mu_en_sur_rho et float mu_sur_rho -> valeur dans les abaques pour une énergie donnée
       * Return : une carte TERMA superposable a la coupe
       *
       */
      std::pair<QImage, QImage> GetKermaMap(int z, float dy, float phi_init_mm2, int beam_width, float angle);
      //QImage GetTermaMap(int z, float ds, float phi_init);


      void PrintDoseSlice(int z);


      QImage PseudoCouleur(int z);

      /*!
       *\fn  qimageFromCoupe()
       *\brief sélection de la coupe afin de créer la QImage
       */
      QImage qimageFromCoupe(int coupe);

      /*!
       *\fn  qimageFromRow()
       *\brief sélection de la ligne afin de créer la QImage
       */
      QImage qimageFromRow(int row);

      /*!
       *\fn  qimageFromCol()
       *\brief sélection de la colonne afin de créer la QImage
       */
      QImage qimageFromCol(int col);

      QImage DensityVol(int z);


      QImage GetDRRView();
 /*!
  *\fn  Methode2
  *\brief Methode 2
  * \arg a_arg1
  * \brief Description de la variable
  * \arg a_arg2
  * \brief Description de la variable
  *
  *
  */
      //void Methode2( Argument a_arg1, Argument a_arg2...); // Le polymorphisme n'est pas géré



      void SetCursor(int i, int j, int k);
      int getCursorI() const { return m_cursor_i; }
      int getCursorJ() const { return m_cursor_j; }
      int getCursorK() const { return m_cursor_k; }




    protected:// si on Hérite, on a accès au protégé
      //------------------------ Constantes Protégées

      //------------------------ Constructeur destructeur Protégés

      //------------------------ Méthodes protégées


    private: // Seule la classe a accès au privé
       //------------------------ Constantes Privées

      //------------------------ Constructeur destructeur Privées

      //------------------------ Méthodes Privées
      /*!
       *\fn  init
       *\brief Initialisation du bloc3D (allocation memoire et mise a zero des elements)
       * \arg a_size
       * \brief nombre de voxels en X, Y et Z
       */
      void init(int a_size);

      /*!
       *\fn  reset
       *\brief Mise a zero des elements du bloc3D
       */
      void reset();

      bloc3d(int XY_size, int Z_size);



      //------------------------ Membres privés (variables ou classes)
 /*!
 * \var m_bloc3D
 * \brief Variable Bloc3D de voxels de meme nombre dans les trois directions
 */
    vector<vector<vector<double> > > m_Bloc3D; //Attention a laisser les espaces sinon le compilateur pense "cin"...
/*!
 * \var m_size
 * \brief Nombre de voxels du Bloc3D (meme nmobre en X, Y mais Z different)
 */
    int m_SizeXY = 512;
    int m_SizeZ ;//192 ici Patient1

    /*!
    * \var m_p_Filenames
    * \brief vtkSmartPointer avec les donnees du patient (nom...)
    */
    //vtkSmartPointer<vtkStringArray> m_p_Filenames;
    /*!
    * \var m_p_ImageData
    * \brief vtkSmartPointer avec les images du patient
    */
   // vtkSmartPointer<vtkImageData>   m_p_ImageData;

    };  // Fin de la classe

#endif // BLOC3D_H
