# 📡 Projet Capteurs - RIOT OS

## 📖 Description

Ce projet implémente une application embarquée sous **RIOT OS** permettant de récupérer et d'afficher les mesures de plusieurs capteurs environnementaux. L'application repose sur un **shell interactif** qui permet de commander différents capteurs via des commandes spécifiques.

Elle utilise **RIOT OS**, un système d’exploitation léger conçu pour les objets connectés et les systèmes embarqués, optimisé pour la gestion des capteurs et la communication en temps réel.

---

## 🛠️ Matériel & Capteurs Utilisés

Ce projet est conçu pour fonctionner sur une plateforme supportant **RIOT OS**, et exploite les capteurs suivants :

1. **LPS331AP** 🏔️ (Capteur de pression et température)
   - Mesure la **température** en degrés Celsius (°C).
   - Mesure la **pression atmosphérique** en hectoPascal (hPa).

2. **LSM303DLHC** 📡 (Accéléromètre & Magnétomètre)
   - Fournit des mesures en **X, Y et Z** pour l'accélération.
   - Fournit des mesures pour le champ magnétique ambiant.

3. **L3G4200D** 🔄 (Gyroscope)
   - Mesure la vitesse angulaire selon **X, Y et Z**.

4. **ISL29020** 🌞 (Capteur de lumière)
   - Mesure la luminosité ambiante en **lux**.

---

## 📦 Installation et Configuration

### 🔧 Prérequis

Avant d'installer et d'utiliser ce projet, assure-toi que ton environnement est prêt :

- **RIOT OS** doit être installé sur ton système.
- **GCC** et **Make** sont nécessaires pour la compilation.
- Un **périphérique compatible** (exemple : carte IoT) doit être connecté.
- Un terminal série (ex: `minicom`, `screen`, `picocom` ou `cutecom`) pour interagir avec le shell RIOT.

---

### 📥 Cloner le projet

Commence par récupérer les fichiers sources en clonant ce dépôt :

```sh
git clone https://github.com/ahmadous/fit-iot-lab-riot-sensor.git
cd votre-repo
🏗️ Compilation et Flash du programme
1️⃣ Compiler le projet :

sh
Copier
Modifier
make
2️⃣ Flasher le programme sur la carte cible :

sh
Copier
Modifier
make flash
3️⃣ Lancer un terminal série pour interagir avec l’application :

sh
Copier
Modifier
make term
🚀 Utilisation
Une fois le programme démarré, l'interface shell de RIOT OS permet d'interagir avec les capteurs via des commandes dédiées.

🖥️ Liste des commandes disponibles
✅ Contrôle du capteur de lumière ISL29020
Commande	Description
isl enable	Active le capteur de lumière
isl disable	Désactive le capteur
isl read	Lit l’intensité lumineuse (lux)
✅ Contrôle de l'Accéléromètre/Magnétomètre LSM303DLHC
Commande	Description
lsm start	Démarre la lecture des valeurs de l'accéléromètre et du magnétomètre
lsm stop	Arrête la lecture
Exemple de sortie :

yaml
Copier
Modifier
Accelerometer x: 123 y: -43 z: 980
Magnetometer x: 56 y: 23 z: -78
✅ Contrôle du Gyroscope L3G4200D
Commande	Description
l3g start	Démarre la lecture des valeurs du gyroscope
l3g stop	Arrête la lecture
Exemple de sortie :

yaml
Copier
Modifier
l3g X: -2 Y: 1 Z: 5
✅ Contrôle du Capteur de Température/Pression LPS331AP
Commande	Description
lps temperature	Affiche la température mesurée
lps pressure	Affiche la pression atmosphérique
Exemple de sortie :

makefile
Copier
Modifier
Temperature: 25.4°C
Pressure: 1013hPa
📂 Structure du projet
Le projet est organisé de la manière suivante :

graphql
Copier
Modifier
📁 sensors
│── 📄 main.c          # Code principal avec gestion des capteurs et du shell
│── 📄 Makefile        # Fichier de compilation pour RIOT OS
│── 📄 sensors.ipynb   # Analyse des données issues des capteurs (si applicable)
│── 📄 README.md       # Ce fichier de documentation
│── 📄 LICENSE         # Fichier de licence (MIT, GPL, etc.)
🛠️ Développement et Personnalisation
Si tu souhaites améliorer ou personnaliser ce projet, voici quelques pistes :

Ajouter d'autres capteurs et implémenter leur gestion.
Implémenter la communication avec un serveur distant pour stocker les mesures.
Optimiser la gestion des threads pour améliorer les performances.
💡 Dépannage
❌ Erreur : "No board specified"
Si vous obtenez cette erreur lors de la compilation, assurez-vous d'avoir défini la cible :

sh
Copier
Modifier
BOARD=<votre_carte> make
Exemple pour une carte nucleo-f401re :

sh
Copier
Modifier
BOARD=nucleo-f401re make
❌ Problème de connexion série
Si vous ne voyez pas de sortie sur le terminal :

Vérifiez que votre carte est bien connectée.
Utilisez dmesg | grep ttyUSB (Linux) ou ls /dev/ pour repérer le bon port.
Assurez-vous que make term utilise le bon périphérique série.
👨‍💻 Contribuer au projet
Toute contribution est la bienvenue ! 🎉

🐛 Signaler un bug : Crée un "Issue" sur GitHub.
💡 Proposer une amélioration : Fais un "Pull Request" avec une description détaillée.
✍ Améliorer la documentation : Aide-nous à mieux expliquer le projet !
📜 Licence
Ce projet est distribué sous licence MIT. Tu es libre de l'utiliser, le modifier et le partager tout en mentionnant l'auteur original.

👤 Auteurs
Papa Ahmadou  - GitHub
Contributeurs : Voir la liste des contributeurs
