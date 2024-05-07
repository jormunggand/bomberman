# TODO

## Important
- le personnage doit mourir au contact d'une bombe

- gérer le multijoueur local
- gérer le multijoueur en réseau
- gérer un mode ou le joueur affronte une IA

- rajouter des ennemis


## Moins important
- rajouter des nouveaux bonus
    1. Kick powerup: 
        lorsque le joueur entre en contact avec la bombe, celle-ci est poussée dans la
        direction opposée jusqu'au prochain mur
    2. Punch powerup
        lorsque le joueur appuie sur un certain bouton et en contact avec la bombe, celle-ci
        est déplacée deux blocs plus loin
- faire un meilleur keyboard handler (trop de copy paste)
- rajouter un HUD avec le temps restant

## Fait
- ajouter un splashscreen au début avec un menu
    - Boutons Multijoueur local, Multijoueur en ligne, Joueur contre Ordinteur
- les bonus doivent être détruits par les bombes
- une fois la bombe posée, le joueur ne peut plus la traverser
- apparition de bonus sur la map
- gestion des bonus pour le joueur
- écrire un keyboard handler : https://stackoverflow.com/questions/1252976/how-to-handle-multiple-keypresses-at-once-with-sdl
