/*!
 *\file bloc3d.cpp
 *\brief Fichier source de la classe Bloc 3D
 * \version 0.0
 * \date 27/03/2018
 * \author Xavier Franceries, Pierre Freton, Hubert Caquineau
 * \version 0.1 24/04/2018 XF Modification method "LoadASCII"
 * \ brief permet de creer et remplir un bloc 3D (apres chargement d une image DICOM)
 * Copyright RMGBM
 *****************************************************************************/



//------------------------ INCLUDES
// INCLUDES Systeme / Déclaration de classes


// INCLUDES Standard Librairie / Déclaration de classes
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <QColor>
#include <QImage>
#include <QRgb>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <QPainter>



// INCLUDES QT / Déclaration de classes


// INCLUDES propres au TP/ Déclaration de classes
#include "bloc3d.h"

//******************************************************************************/
//* Constructeur - Destructeur
//******************************************************************************/
 /*!
  *\fn  bloc3d
  *\brief constructeur de la classe (vide)
  */
      bloc3d::bloc3d()
      {

          m_SizeXY = 0;
          m_SizeZ = 0;
          m_Bloc3D.clear();
      }

 /*!
  *\fn  bloc3d
  *\brief constructeur de la classe (bloc de taille a_size)
  */
      bloc3d::bloc3d(int XY_size, int Z_size)
      {
          m_SizeXY = XY_size;
          m_SizeZ = Z_size;
          m_Bloc3D.clear();
       }




      /*!
       *\fn  bloc3d
       *\brief constructeur de la classe (bloc de taille a_size)
       */
           bloc3d::bloc3d(int a_size)
           {
              cout << "-Constructeur deb BLOC3D : Adresse classe = %h " << (this) << endl;
              init(a_size);
              cout << "-Constructeur fin BLOC3D : Adresse classe = %h " << (this) << endl;
            }




 /*!
  *\fn  ~bloc3d
  *\brief Destructeur de la classe
  */

      bloc3d::~bloc3d()
      {


      }


//******************************************************************************/
//* Méthodes publiques
//******************************************************************************/

/*!
  *\fn  load Dicom
  *\brief chargement de l image DICOM et mise dans le bloc 3D
  */
      bool bloc3d::loadDicom()
      {
          /*
          // Read using vtkDICOMReader
          vtkSmartPointer<vtkDICOMReader> l_p_reader = vtkSmartPointer<vtkDICOMReader>::New();
          l_p_reader->SetFileNames( m_p_filenames );
          l_p_reader->SetMemoryRowOrderToFileNative();
          l_p_reader->AutoRescaleOff();
          l_p_reader->Update();
          */
      }


      /*!
        *\fn  load ASCII
        *\brief chargement de l image au format ASCII et mise dans le bloc 3D
        */
            void bloc3d::loadASCII( string a_prefix ) //prefix contient le chemin et début du nom des coupes images à charger
            {

                m_Bloc3D.clear();//obligatoire pour initiser la variable avant de la remplir

                string ligne;

                vector<double> row;
                vector<vector<double> > coupe;
                double val;
                cout<<" Début de la boucle sur les coupes "<<endl;
                for(int k = 0; k < 191; k++)
                //for(int k = 0; k < 19; k++)
                {

                    ostringstream stream;
                    stream << k+1;

                    string path;

                    //to consider order of axial images (001,002,003...)
                    if (k < 9)//start at 0
                    {
                       path = a_prefix + "00" + stream.str() + ".txt";
                    }
                    else
                    {
                        if (k < 99)//start at 0
                        {
                            path = a_prefix + "0" + stream.str() + ".txt";
                        }
                        else
                        {
                            path = a_prefix + stream.str() + ".txt";
                        }
                    }



                    cout << path << endl;

                    ifstream fichier(path.c_str(),  ifstream::in);

                    if(fichier)
                    {
                        // lecture du nombre d'éléments du vecteur
                        for (int i = 0; i < 512; i++)
                        {
                          for(int j = 0; j < 511; j++)
                          {
                              getline(fichier, ligne, '\t');
                              istringstream is(ligne);

                              is >> val;

                              row.push_back(val);
                          }
                          getline(fichier, ligne, '\n');
                          istringstream is(ligne);
                          is >> val;
                          row.push_back(val);


                          coupe.push_back(row);
                          row.clear();
                        }
                        m_Bloc3D.push_back(coupe);
                        coupe.clear();
                    }
                    else
                      {
                        cout << "erreur fichier" << endl;
                      }
                }

                cout << "Tout a ete charge " << endl;

                m_SizeZ = 192;//19;//192;
                cout<<" On sort de la fonction loadASCII "<<endl;
            }



      /*!
        *\fn  reset
        *\brief Mise à zéro de tout le volume
        */
      void bloc3d::reset()
      {
          cout << "PASSAGE reset debut bloc3D " << endl;
cout << "-Cpassage Reset dans la classe Bloc3D a l'adresse = %h " << (this) << endl;
          if (!m_Bloc3D.empty() && m_SizeZ!=0)//security vecteur vide...
          {
              int i,j,k;
              for(i=0; i<m_SizeXY;i++)
                  for(j=0; j<m_SizeXY;j++)
                     for(k=0; k<m_SizeZ;k++)
                     {
                         m_Bloc3D[i][j][k] = 0;//i+j+k;
                         cout << "---test B["<< i << "]["<< j << "][" << k << "] = " << m_Bloc3D[i][j][k] << endl;
                     }
          }
          cout << "PASSAGE reset fin bloc3D " << endl;
      }


      /*!
        *\fn  qimageFromCoupe
        *\brief sélection de la coupe afin de créer la QImage
        */
      QImage bloc3d::qimageFromCoupe(int coupe)
      {
          QImage img(512, 512, QImage::Format_RGB16);

          // Sécurité : vérifier que la coupe demandée existe
          if (coupe < 0 || coupe >= m_Bloc3D.size()) {
              std::cerr << "Erreur : indice de coupe " << coupe << " invalide (hors bornes)" << std::endl;
              return img; // image vide
          }

          vector<vector<double>> pixCoupe = m_Bloc3D.at(coupe);

          // Sécurité : vérifier qu'on a bien 512 lignes
          if (pixCoupe.size() < 512) {
              std::cerr << "Erreur : coupe " << coupe << " a seulement " << pixCoupe.size() << " lignes" << std::endl;
              return img;
          }

          for (int i = 0; i < 512; i++) {
              vector<double> pixRow = pixCoupe.at(i);

              // Sécurité : vérifier qu'on a bien 512 colonnes
              if (pixRow.size() < 512) {
                  std::cerr << "Erreur : ligne " << i << " de la coupe " << coupe << " a seulement " << pixRow.size() << " colonnes" << std::endl;
                  continue; // on saute cette ligne
              }

              for (int j = 0; j < 512; j++) {
                  double val = 255 * (pixRow.at(j) / 4096.0); // normalisation
                  img.setPixel(j, i, qRgb(val, val, val));
              }
          }

          // Affichage du curseur rouge
          QPainter painter(&img);
          painter.setPen(QPen(Qt::red, 1));
          painter.drawLine(0, m_cursor_j, 511, m_cursor_j); // ligne horizontale
          painter.drawLine(m_cursor_i, 0, m_cursor_i, 511); // ligne verticale

          return img;
      }

 QImage bloc3d::GetCorView(int y){

     int profondeur = m_Bloc3D.size();
     int largeur  = m_Bloc3D.at(0).at(0).size();
     int hauteur = m_Bloc3D.at(0).size();

     QImage img(largeur, profondeur, QImage::Format_RGB16);

     int delta_z = (512 - profondeur)/2;



     for(int z = 0 ; z < profondeur; z++){
         for(int x = 0 ; x < largeur; x++){


            double val = 255 * m_Bloc3D.at(z).at(y).at(x) / 4096;
            img.setPixel(x, z , qRgb(val, val, val));

         }
     }img = img.scaled(largeur,hauteur,Qt::IgnoreAspectRatio);


     QPainter painter(&img);
     painter.setPen(QPen(Qt::red, 1));
     float scale_factor =  (512.0f / 191.0f);
     int y_k = m_cursor_k * scale_factor;

     if (y_k == 0){
         painter.drawLine(0, 1, 511, 1);
     }
     else if (y_k == 511){
         painter.drawLine(0, 510, 511, 510);
     }
     painter.drawLine(0, y_k, 511, y_k); // ligne horizontale
     painter.drawLine(m_cursor_i, 0, m_cursor_i, 511); // ligne verticale

     return img;
 }

 QImage bloc3d::GetSagView(int x){
     int profondeur = m_Bloc3D.size();
     int largeur  = m_Bloc3D[0][0].size();
     int hauteur = m_Bloc3D.at(0).size();

     QImage img(largeur, profondeur, QImage::Format_RGB16);

     int delta_z = (512 - profondeur)/2;

     for(int z = 0 ; z < profondeur; z++)
     {
         for(int y = 0 ; y < largeur; y++)
         {

            int val = 255 * m_Bloc3D.at(z).at(y).at(x) / 4096;

            img.setPixel(y, z, qRgb(val, val, val));
         }
     }img = img.scaled(largeur,hauteur,Qt::IgnoreAspectRatio);


     QPainter painter(&img);
     painter.setPen(QPen(Qt::red, 1));
     float scale_factor =  (512.0f / 191.0f);
     int y_k = m_cursor_k * scale_factor;

     if (y_k == 0){
         painter.drawLine(0, 1, 511, 1);
     }
     else if (y_k == 511){
         painter.drawLine(0, 510, 511, 510);
     }
     painter.drawLine(0, y_k, 511, y_k ); // ligne horizontale
     painter.drawLine(m_cursor_j, 0, m_cursor_j, 511); // ligne verticale

     return img;
 }


 QRgb getColorFromDose(float normVal)
 {
     normVal = std::max(0.0f, std::min(normVal, 1.0f)); // clamp

     int r = 0, g = 0, b = 0;

     if (normVal < 0.2f) {
         float t = normVal / 0.2f;
         r = 0;
         g = static_cast<int>(255 * t);
         b = static_cast<int>(139 * (1 - t));
     } else if (normVal < 0.5f) {
         float t = (normVal - 0.2f) / 0.3f;
         r = static_cast<int>(255 * t);
         g = 255;
         b = 0;
     } else if (normVal < 0.8f) {
         float t = (normVal - 0.5f) / 0.3f;
         r = 255;
         g = static_cast<int>(255 * (1 - t));
         b = 0;
     } else {
         float t = (normVal - 0.8f) / 0.2f;
         r = static_cast<int>(255 * (1 - t) + 148 * t);
         g = 0;
         b = static_cast<int>(0 + 211 * t);
     }

     return qRgba(r, g, b, 160);

 }


 QRgb getIsoColor(float dose_gy)
 {
     if (dose_gy < 0.2f)
         return qRgba(0, 0, 0, 0); // transparent
     else if (dose_gy < 0.4f)
         return qRgba(0, 0, 255, 160); // bleu
     else if (dose_gy < 0.6)
         return qRgba(0, 255, 0, 160); // vert
     else if (dose_gy < 0.85f)
         return qRgba(255, 255, 0, 160); // jaune
     else
         return qRgba(255, 0, 0, 160); // rouge
 }



void Normalize(std::vector<std::vector<double>> Image)
 {

     const int largeur = 512;
     const int hauteur = 512;
     double MinVal = 0;
     double MaxVal = 0;

     for (int x = 0 ; x < largeur; x++ )
     {
         for (int y = 0; y < hauteur; ++y)
         {
             MinVal = std::min(MinVal,Image[y][x]);
             MaxVal = std::min(MaxVal,Image[y][x]);
         }
     }

     for (int x = 0 ; x < largeur; x++ )
     {
         for (int y = 0; y < hauteur; ++y)
         {

            Image[y][x] =  (Image[y][x] - MinVal)/MaxVal * 1000;

         }
     }

 }

QImage bloc3d::DensityVol(int z)
{
    const int largeur = 512;
    const int longueur = 512;

    std::vector<std::vector<double>> HU = m_Bloc3D.at(z);
    QImage DensityVolume(512, 512, QImage::Format_RGB16);
    for (int x = 0; x < largeur; ++x)
    {
        for (int y = 0; y < longueur; ++y)
        {
            float hu = HU[y][x];
            double rho = ( ((hu - 1024.0f) * 0.001f) + 1.0f) * 100;
            DensityVolume.setPixel(x, y, qRgb(rho, rho, rho));
        }
    }
    return DensityVolume;
}
std::vector<std::vector<float>> AnalyticalKernel(int size, float voxelSize_mm)
{
    std::vector<std::vector<float>> kernel(size, std::vector<float>(size, 0.0f));
    int center = size / 2;
    float sum = 0.0f;

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            int dx = j - center;
            int dy = i - center;
            float d = std::sqrt(dx * dx + dy * dy) * voxelSize_mm / 10.0f;


            float val = 0.0f;
            if (d > 0.0f)
                val = 3.26e-6f * std::exp(-0.08f * d) / (d * d);
            else
                val = 0.0f;

            kernel[i][j] = val;

        }
    }
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            sum += kernel[i][j];

    if (sum > 0.0f)
    {
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                kernel[i][j] /= sum;
    }



    return kernel;
}



std::vector<std::vector<float>> ApplyKernel(std::vector<std::vector<float>>& TERMA,
                                            std::vector<std::vector<float>>& Kernel)
{
    int hauteur = TERMA.size();
    int largeur = TERMA[0].size();
    int kSize = Kernel.size();
    int kCenter = kSize / 2;

    std::vector<std::vector<float>> KERMA(hauteur, std::vector<float>(largeur, 0.0f));

    auto itKermaY = KERMA.begin();
    auto itTERMAY = TERMA.begin();

    for (int y = 0; itKermaY != KERMA.end(); ++itKermaY, ++itTERMAY, ++y)
    {
        auto& ligneKerma = *itKermaY;

        for (int x = 0; x < largeur; ++x)
        {
            float somme = 0.0f;

            for (int i = 0; i < kSize; ++i)
            {
                int yi = y + i - kCenter;
                if (yi < 0 || yi >= hauteur)
                    continue;

                auto& ligneTERMA_kernel = TERMA[yi];
                auto& ligneKernel = Kernel[i];

                for (int j = 0; j < kSize; ++j)
                {
                    int xi = x + j - kCenter;
                    if (xi < 0 || xi >= largeur)
                        continue;

                    somme += ligneTERMA_kernel[xi] * ligneKernel[j];
                }
            }

            ligneKerma[x] = somme;
        }
    }

    return KERMA;
}




//===========================================================================================//

std::pair<QImage, QImage> bloc3d::GetKermaMap(int z, float dy, float phi_init_mm2, int beam_width, float angle)
{

    const int largeur = 512;
    const int hauteur = 512;
    const float voxelSize_mm = 1.0f;
    const float energy_MeV = 2.0f;
    const float MeV_to_Gy = 1.0f / 6.24e9f;  // 1 Gy = 6.24e9 MeV/g

    // --- Chargement de la coupe CT
    std::vector<std::vector<double>> coupe = m_Bloc3D.at(z);
    std::vector<std::vector<float>> TERMA(hauteur, std::vector<float>(largeur, 0.0f));

    // --- Paramètres du faisceau
    int centre_faisceau = largeur / 2;
    int demi_largeur = beam_width / 2;

    // --- TERMA : μ_en/ρ × fluence × énergie
    for (int x = 0; x < largeur; ++x)
    {
        bool in_beam = std::abs(x - centre_faisceau) <= demi_largeur;
        float attenuation = 0.0f;

        for (int y = 0; y < hauteur; ++y)
        {
            float hu = coupe[y][x];

            // --- Correction densité pour éviter rho < 0
            float rho = std::max(0.001f, ((hu - 1024.0f) * 0.001f) + 1.0f); // rho = gm/cm3

            // --- Choix μ_en/ρ (en cm²/g) selon HU
            float mu_en_sur_rho = 0.030f; // tissus mous par défaut
            if (hu < 400)        mu_en_sur_rho = 0.026f; // air
            else if (hu < 1010)  mu_en_sur_rho = 0.030f; // tissus mous
            else if (hu < 1200)  mu_en_sur_rho = 0.035f; // os trabéculaire
            else                 mu_en_sur_rho = 0.038f; // os cortical

            float mu = mu_en_sur_rho * rho;  // coefficient d'atténuation total μ (cm⁻¹)
            attenuation += mu * dy; // dy en cm-1

            if (in_beam)
            {
                // --- Correction : phi_init_mm2 est une fluence surfacique
                float fluence = phi_init_mm2 * std::exp(-attenuation); // particules / mm2

                // --- TERMA en MeV/g (mu_en_sur_rho est en cm²/g, fluence en /mm² → conversion mm² → cm²)
                TERMA[y][x] = mu_en_sur_rho * fluence * energy_MeV * 100.0f; // (×100 pour mm² → cm²), MeV / g
            }
            else
            {
                TERMA[y][x] = 0.0f;
            }
        }
    }

    // --- Application du noyau analytique pour calcul du KERMA
    auto kernel = AnalyticalKernel(8, voxelSize_mm);
    auto KERMA = ApplyKernel(TERMA, kernel); // toujours en MeV/g

    // --- Conversion KERMA en Gray
    std::vector<std::vector<double>> physical_Kerma(hauteur, std::vector<double>(largeur, 0.0f));
    for (int y = 0; y < hauteur; ++y)
    {
        for (int x = 0; x < largeur; ++x)
        {
            float kerma_MeVg = KERMA[y][x]; // MeV/g
            physical_Kerma[y][x] = kerma_MeVg * MeV_to_Gy; // en Gy
        }
    }

    if (z >= static_cast<int>(m_PhysicalKerma.size()))
        m_PhysicalKerma.resize(z + 1);
    m_PhysicalKerma[z] = physical_Kerma;

    // --- Recherche du maximum pour normalisation couleur
    float maxVal = 0.0f;
    for (const auto& ligne : KERMA)
        for (float v : ligne)
            maxVal = std::max(maxVal, v);
    if (maxVal == 0.0f) maxVal = 1.0f;

    // --- Affichage fusionné CT + Dose
    QImage image_Gradient(largeur, hauteur, QImage::Format_ARGB32);
    for (int y = 0; y < hauteur; ++y)
    {
        for (int x = 0; x < largeur; ++x)
        {
            float valCT = coupe[y][x];
            float dose = KERMA[y][x];

            int ng = static_cast<int>(255.0f * valCT / 4096.0f);
            ng = qBound(0, ng, 255);

            if (dose == 0.0f)
            {
                image_Gradient.setPixel(x, y, (valCT < 400) ? qRgb(0, 0, 0) : qRgb(ng, ng, ng));
            }
            else
            {
                float norm = dose / maxVal;
                QRgb couleur = getColorFromDose(norm);
                float alpha = 0.35f;
                int r = static_cast<int>((1 - alpha) * ng + alpha * qRed(couleur));
                int g = static_cast<int>((1 - alpha) * ng + alpha * qGreen(couleur));
                int b = static_cast<int>((1 - alpha) * ng + alpha * qBlue(couleur));
                image_Gradient.setPixel(x, y, qRgb(r, g, b));
            }
        }
    }
    QImage Image_iso(largeur, hauteur, QImage::Format_ARGB32);
    for (int y = 0; y < hauteur; ++y)
    {
        for (int x = 0; x < largeur; ++x)
        {
            float valCT = coupe[y][x];
            float dose = KERMA[y][x];

            int ng = static_cast<int>(255.0f * valCT / 4096.0f);
            ng = qBound(0, ng, 255);

            if (dose == 0.0f)
            {
                Image_iso.setPixel(x, y, (valCT < 400) ? qRgb(0, 0, 0) : qRgb(ng, ng, ng));
            }
            else
            {
                float norm = dose / maxVal;
                QRgb couleur = getIsoColor(norm);
                float alpha = 0.35f;
                int r = static_cast<int>((1 - alpha) * ng + alpha * qRed(couleur));
                int g = static_cast<int>((1 - alpha) * ng + alpha * qGreen(couleur));
                int b = static_cast<int>((1 - alpha) * ng + alpha * qBlue(couleur));
                Image_iso.setPixel(x, y, qRgb(r, g, b));
            }
        }
    }

    return {image_Gradient, Image_iso};
}

//===========================================================================================//
/*
// Fonction tierce : Ray tracing avec Siddon dans une coupe 2D
struct SiddonRay {
    std::vector<std::pair<int, int>> voxels;
    std::vector<float> lengths;
};

SiddonRay RayTraceSiddon(float x0, float y0, float x1, float y1, int largeur, int hauteur, float voxelSize_mm)
{
    SiddonRay ray;

    float dx = x1 - x0;
    float dy = y1 - y0;
    float L = std::sqrt(dx * dx + dy * dy);
    if (L == 0.0f) return ray;

    dx /= L;
    dy /= L;

    float x = x0;
    float y = y0;

    for (float t = 0.0f; t < L; t += voxelSize_mm) {
        int ix = static_cast<int>(x);
        int iy = static_cast<int>(y);

        if (ix >= 0 && ix < largeur && iy >= 0 && iy < hauteur) {
            ray.voxels.emplace_back(iy, ix);
            ray.lengths.push_back(voxelSize_mm);
        }

        x += dx * voxelSize_mm;
        y += dy * voxelSize_mm;
    }

    return ray;
}

// Nouvelle version de GetKermaMap avec rotation libre et Siddon
QImage bloc3d::GetKermaMap(int z, float dy, float phi_init_mm2, int beam_width, float angle_deg, float cx, float cy)
{
    const int largeur = 512;
    const int hauteur = 512;
    const float voxelSize_mm = 1.0f;
    const float energy_MeV = 2.0f;
    const float MeV_to_Gy = 1.0f / 6.24e9f;

    std::vector<std::vector<double>> coupe = m_Bloc3D.at(z);
    std::vector<std::vector<float>> TERMA(hauteur, std::vector<float>(largeur, 0.0f));

    float angle_rad = angle_deg * M_PI / 180.0f;
    float dx = std::cos(angle_rad);
    float dy_rot = std::sin(angle_rad);

    float phi_init = phi_init_mm2 / (beam_width * voxelSize_mm); // par rayon

    // Position de la source (à distance fixe du centre de rotation)
    float R = std::max(largeur, hauteur) * 1.5f; // distance fictive, faisceau incident en bord d'image
    float sx = cx - R * dx;
    float sy = cy - R * dy_rot;

    for (int i = -beam_width/2; i <= beam_width/2; ++i) {
        float px = cx + i * dy_rot;
        float py = cy - i * dx;

        SiddonRay ray = RayTraceSiddon(sx, sy, px, py, largeur, hauteur, voxelSize_mm);

        float attenuation = 0.0f;
        for (size_t k = 0; k < ray.voxels.size(); ++k) {
            int y = ray.voxels[k].first;
            int x = ray.voxels[k].second;

            float hu = coupe[y][x];
            float rho = ((hu - 1024.0f) * 0.001f) + 1.0f;

            float mu_en_sur_rho = 0.030f;
            if (hu < 400)        mu_en_sur_rho = 0.026f;
            else if (hu < 1010)  mu_en_sur_rho = 0.030f;
            else if (hu < 1200)  mu_en_sur_rho = 0.035f;
            else                 mu_en_sur_rho = 0.038f;

            float mu = mu_en_sur_rho * rho;
            float dL = ray.lengths[k];
            attenuation += mu * dL;

            float fluence = phi_init * std::exp(-attenuation);
            float terma = mu_en_sur_rho * fluence * energy_MeV; // en MeV/g
            TERMA[y][x] += terma;
        }
    }

    auto kernel = AnalyticalKernel(15, voxelSize_mm);
    auto KERMA = ApplyKernel(TERMA, kernel);

    std::vector<std::vector<double>> physical_Kerma(hauteur, std::vector<double>(largeur, 0.0f));
    for (int y = 0; y < hauteur; ++y)
        for (int x = 0; x < largeur; ++x)
            physical_Kerma[y][x] = KERMA[y][x] * MeV_to_Gy;

    if (z >= static_cast<int>(m_PhysicalKerma.size()))
        m_PhysicalKerma.resize(z + 1);
    m_PhysicalKerma[z] = physical_Kerma;

    // --- Affichage
    float maxVal = 0.0f;
    for (auto& row : KERMA)
        for (float v : row)
            maxVal = std::max(maxVal, v);
    if (maxVal == 0.0f) maxVal = 1.0f;

    QImage image(largeur, hauteur, QImage::Format_ARGB32);
    for (int y = 0; y < hauteur; ++y)
    {
        for (int x = 0; x < largeur; ++x)
        {
            float valCT = coupe[y][x];
            float dose = KERMA[y][x];
            int ng = static_cast<int>(255.0f * valCT / 4096.0f);
            ng = qBound(0, ng, 255);

            if (dose == 0.0f)
            {
                image.setPixel(x, y, (valCT < 400) ? qRgb(0, 0, 0) : qRgb(ng, ng, ng));
            }
            else
            {
                float norm = dose / maxVal;
                QRgb couleur = getColorFromDose(norm);
                float alpha = 0.35f;
                int r = static_cast<int>((1 - alpha) * ng + alpha * qRed(couleur));
                int g = static_cast<int>((1 - alpha) * ng + alpha * qGreen(couleur));
                int b = static_cast<int>((1 - alpha) * ng + alpha * qBlue(couleur));
                image.setPixel(x, y, qRgb(r, g, b));
            }
        }
    }

    return image;
}

*/
//===========================================================================================//
void bloc3d::PrintDoseSlice(int z)
{
    if (z < 0 || z >= static_cast<int>(m_PhysicalKerma.size())) {
        std::cout << "Indice z invalide : " << z << std::endl;
        return;
    }

    const auto& slice = m_PhysicalKerma[z];

    std::cout << "==== Dose en Gy pour la coupe z = " << z << " ====" << std::endl;
    for (const auto& row : slice) {
        for (double dose : row) {
            std::cout << std::fixed << std::setprecision(4) << dose << " ";
        }
        std::cout << std::endl;
    }
}




QImage bloc3d::PseudoCouleur(int z)
{
    const int largeur = 512;
    const int hauteur = 512;
    QImage ImageCouleur(largeur, hauteur, QImage::Format_ARGB32);

    std::vector<std::vector<double>> coupe = m_Bloc3D.at(z);

    for (int y = 0; y < hauteur; ++y) {
        for (int x = 0; x < largeur; ++x) {


            float hu = coupe[y][x];
            float rho = ((hu - 1024.0f) * 0.001f) + 1.0f;

            float mu_sur_rho_local, mu_en_sur_rho_local;

            if (hu < 400)
            { //Air
                ImageCouleur.setPixel(x, y, qRgb(0, 0, 0)); // transparent
            }
            else if (hu < 1010)
            { // Tissus mou
                ImageCouleur.setPixel(x, y, qRgb(100, 140, 180));  // rouge
            }
            else if ( hu < 1200)
            { //  Graisse
                ImageCouleur.setPixel(x, y, qRgb(210, 180, 100));  // vert
            }
            else
            { // Os
                ImageCouleur.setPixel(x, y, qRgb(190, 130, 120));  // bleu

            }
        }
   }
    return ImageCouleur;
}

QImage bloc3d::GetDRRView() {
     int profondeur = m_Bloc3D.size();
     int largeur = m_Bloc3D[0][0].size();
     int hauteur = m_Bloc3D[0].size();

     QImage img(largeur, profondeur, QImage::Format_RGB16);
     img.fill(0);
    int delta_z = (largeur - profondeur) / 2;
     // Parcours des plans y et z (projection coronale)
     for (int z =0; z < profondeur; z++) {
         for (int x = 0; x < largeur; x++) {
             double somme = 0.0;
             // Somme des intensités sur X
             for (int y = 0; y < hauteur; y++) {
                 somme += m_Bloc3D.at(z).at(y).at(x);
             }
             // Normalisation pour affichage (mise à l'échelle entre 0 et 255)
             double val = 255.0*(somme/(4096.0*profondeur));
             val = std::min(255.0, std::max(0.0, val));
             img.setPixel(x, z, qRgb(val, val, val));
         }
     } img = img.scaled(hauteur,largeur, Qt::IgnoreAspectRatio);
     return img;
}


float bloc3d::GetDoseAt(int x, int y, int z) const {
    if (z >= 0 && z < static_cast<int>(m_PhysicalKerma.size()) &&
        y >= 0 && y < static_cast<int>(m_PhysicalKerma[z].size()) &&
        x >= 0 && x < static_cast<int>(m_PhysicalKerma[z][y].size())) {

        return m_PhysicalKerma[z][y][x];
    }
    return 0.0f;
}


void bloc3d::SetCursor(int i, int j, int k)
{
    m_cursor_i = i;
    m_cursor_j = j;
    m_cursor_k = k ;


}


//******************************************************************************/
//* Méthodes protégées
//******************************************************************************/


//******************************************************************************/
//* Méthodes privées
//******************************************************************************/
      /*!
       *\fn  init
       *\brief Methode d initialisation du bloc3D (allocation et mise a zero)
       * \arg a_size
       *  \brief taille du bloc (nombre de voxels identiques en X,Y,Z)
       */
      void bloc3d::init(int a_size)
      {
          cout << "PASSAGE init debut bloc3D : a_size=" << a_size << endl;

          m_SizeXY = a_size;
          m_SizeZ  = a_size;

          m_Bloc3D.resize(m_SizeXY);
          int l_i,l_j;
          for(l_i=0; l_i<m_SizeXY;l_i++)
          {
              m_Bloc3D[l_i].resize(m_SizeXY);
              for(l_j=0; l_j<m_SizeZ;l_j++)
                  m_Bloc3D[l_i][l_j].resize(m_SizeZ);
          }



          reset();
          cout << "PASSAGE init fin bloc3D " << endl;
      }





