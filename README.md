# TP3 SM57 - Systeme embarque STM32F407

## Objectif general
Ce projet implemente une application embarquee sur STM32F407 pour manipuler plusieurs peripheriques temps reel:
- acquisition capteur (accelerometre)
- communication serie (USART2)
- communication I2C et SPI
- affichage LCD
- generation analogique via DAC
- gestion date/heure via RTC

L objectif typique est de construire une chaine complete:
mesure -> traitement -> affichage/commande.

## Plateforme
- MCU: STM32F407VGTX
- Outil: STM32CubeIDE
- Config materielle: fichier .ioc
- HAL STM32F4 + CMSIS

## Organisation du code
- Core/Inc: interfaces modules
- Core/Src: implementation des modules
- Core/Startup: startup assembleur et table des interruptions
- Drivers: HAL + CMSIS
- Debug: artefacts de compilation auto generes

## Modules principaux
- accelerometre.c/h
  Lecture et logique associee au capteur d acceleration.
- i2c.c/h
  Initialisation et echanges I2C (souvent utilise pour capteurs).
- spi.c/h
  Initialisation et echanges SPI (selon le composant cible).
- lcd_displaytech162.c/h
  Pilote d affichage LCD.
- dac.c/h
  Pilotage de la sortie analogique.
- rtc.c/h
  Configuration et usage de l horloge temps reel.
- usart2.c/h
  Port serie pour debug, traces ou commandes.

## Flux fonctionnel type
1. Initialisation HAL, horloge systeme, GPIO et peripheriques.
2. Acquisition d une mesure capteur (accelerometre).
3. Eventuel traitement/formatage des donnees.
4. Affichage local (LCD) et/ou emission sur liaison serie.
5. Eventuelle conversion en sortie analogique (DAC).
6. Horodatage via RTC si necessaire.

## Fichiers de configuration importants
- Projet TP3 SM57.ioc: source de verite de la configuration CubeMX.
- STM32F407VGTX_FLASH.ld: edition de liens execution Flash.
- STM32F407VGTX_RAM.ld: edition de liens execution RAM.

## Compilation et debug
- Ouvrir le projet dans STM32CubeIDE.
- Verifier la configuration cible (Debug).
- Compiler puis flasher sur la carte STM32F407.
- Utiliser la console serie USART2 pour traces runtime.

## Notes de maintenance
- Eviter de modifier manuellement les sections auto generees CubeMX hors zones USER CODE.
- Garder la coherence entre .ioc et le code regenere.
- Documenter dans ce README les changements de logique applicative.
