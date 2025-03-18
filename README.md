# 📡 Projet Capteurs - RIOT OS

Ce projet implémente une application embarquée sous **RIOT OS** permettant de récupérer et d'afficher les mesures de plusieurs capteurs environnementaux, notamment :

- **LPS331AP** : Capteur de température et pression atmosphérique.
- **LSM303DLHC** : Accéléromètre et magnétomètre.
- **L3G4200D** : Gyroscope.
- **ISL29020** : Capteur de luminosité.

## 📦 Installation

### Prérequis
- **RIOT OS** installé sur votre système.
- Un environnement de compilation configuré avec **GCC** et **Make**.
- Un périphérique compatible (ex : carte IoT avec support RIOT OS).

### Compilation et Flash
1. Cloner le dépôt :
   ```sh
   git clone https://github.com/ahmadous/fit-iot-lab-riot-sensor.git
   cd votre-repo
Compiler le projet :
sh
Copier
Modifier
make
Flasher l'application sur la carte cible :
sh
Copier
Modifier
make flash
🚀 Utilisation
Une fois le programme lancé, vous pouvez interagir avec les capteurs via l'interface shell de RIOT OS.

Commandes disponibles :
Capteur de lumière ISL29020

isl enable : Activer le capteur.
isl disable : Désactiver le capteur.
isl read : Lire la luminosité (en lux).
Accéléromètre/Magnétomètre LSM303DLHC

lsm start : Démarrer la lecture des valeurs.
lsm stop : Arrêter la lecture.
Gyroscope L3G4200D

l3g start : Démarrer la lecture des valeurs.
l3g stop : Arrêter la lecture.
Capteur de pression/température LPS331AP

lps temperature : Lire la température (°C).
lps pressure : Lire la pression atmosphérique (hPa).
🛠 Structure du projet
main.c : Contient l'implémentation principale de l'application.
Makefile : Fichier de compilation spécifique à RIOT OS.
sensors.ipynb : Notebook d'analyse des données (si applicable).
📝 Auteurs
Votre Nom (@ahmadous)
