# Projet Accelerometre, RTC et DAC - STM32F407

## Contexte general
Ce projet est un TP embarque autour de la carte STM32F407.
L idee est de valider une chaine de traitement complete en microcontroleur:
- acquisition de donnees capteur
- traitement applicatif
- restitution locale (affichage/serie)
- sortie analogique
- horodatage des mesures

Le projet sert donc de base d apprentissage pour l integration de plusieurs peripheriques sur une meme application temps reel simple.

## Objectif pedagogique
L objectif principal est de pratiquer:
- la configuration CubeMX/CubeIDE
- l usage de drivers HAL sur plusieurs bus (I2C, SPI, USART)
- la lecture capteur et l exploitation des donnees
- la synchronisation avec l horloge RTC
- la generation d une sortie analogique via DAC

## Perimetre fonctionnel
L application manipule trois briques metier centrales:
- Accelerometre: acquisition de mouvement/orientation
- RTC: gestion de la date/heure
- DAC: production d un signal analogique lie a la logique applicative

Autour de ces briques, des interfaces de communication (serie, bus capteur, affichage) permettent l observation et le debug.

## Structure globale
Le depot suit la structure standard STM32CubeIDE:
- code applicatif dans `Core`
- couches constructeur dans `Drivers`
- configuration materielle dans le fichier `.ioc`

## Utilisation rapide
1. Ouvrir le projet dans STM32CubeIDE.
2. Verifier la configuration cible (Debug).
3. Compiler puis flasher la carte STM32F407.
4. Observer le comportement via la liaison serie et/ou l affichage.

## Notes de maintenance
- Conserver les modifications utilisateur dans les zones USER CODE.
- Garder le fichier `.ioc` coherent avec le code regenere.
- Maintenir ce README centre sur le contexte, les objectifs et le comportement global.
