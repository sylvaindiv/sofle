# Sofle RGB — Vial firmware & éditeur

Configuration Vial pour clavier **Sofle Choc** avec RGB per-key, prête à forker.

Ce repo est public et librement réutilisable pour builder son propre Sofle avec RGB par touche.

## Contenu

- `*.uf2` — firmware Vial précompilé à flasher sur le clavier
- `index.html`, `vial-rgb-editor_1.html` — éditeur RGB per-key (standalone, s'ouvre dans le navigateur)
- `sofle-layout-mapper.html` / `sofle-layout-map.json` — mapping du layout clavier
- `netlify-deploy/` — version déployée sur Netlify

## Flasher le firmware

1. Mets le clavier en mode bootloader (bouton reset / double-tap selon le PCB).
2. Copie le fichier `.uf2` sur le lecteur qui apparaît (`RPI-RP2` ou équivalent).
3. Le clavier redémarre automatiquement avec le nouveau firmware.

## Configurer le RGB

Le firmware supporte [Vial](https://get.vial.today/) pour les keymaps. Pour le RGB per-key, ouvre `index.html` (ou `vial-rgb-editor_1.html`) dans un navigateur — aucune installation requise.

## Forker

Fork ce repo, remplace le `.uf2` par ton propre build si besoin, et adapte `sofle-layout-map.json` à ton layout.
