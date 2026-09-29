#!/usr/bin/env bash
# Downloads the photos and renders from the Printables page into docs/images/.
# Run once from the repository root (works in Git Bash on Windows):
#   bash docs/fetch_images.sh
# Afterwards commit docs/images/ and delete this script if you like.
set -e
cd "$(dirname "$0")/images"
M=https://media.printables.com/media/prints
R=https://media.printables.com/media/prints/1565668/rich_content
get() { echo "  $1"; curl -sSfL -A "Mozilla/5.0" -o "$1" "$2"; }

echo "Renders"
get hero.png      "$M/81381c71-2b87-45eb-af55-97f0769b2278/images/12871540_713ebd79-058b-477e-a4cf-34647079cad8_6439b551-45c2-4ac2-ba36-ea304913e2dc/thumbs/cover/1200x630/png/printablesmakerworld-3d-tetris11.png"
get gallery-1.png "$M/b866ebc0-2e49-4fe2-b466-d91df7221e72/images/12871541_6c94e5e2-3a3c-42fe-b765-6de7379fb41f_385bf363-2fd9-494e-821f-80141de0ff88/thumbs/cover/800x800/png/printablesmakerworld-3d-tetris12.png"
get gallery-2.png "$M/ad420370-fbc4-4f10-97c1-39592c35d7bb/images/12871566_2efdce83-9f17-45e4-8b6d-7f161a6cd77f_85d11447-d7a4-45e9-9944-bc9a0fdba369/thumbs/cover/800x800/png/6.png"
get gallery-3.png "$M/7ed404ab-8cac-4061-aadc-e78a33cb7474/images/12871565_977bc880-faf6-4ec4-86ac-eef90e4369ae_91cc3534-e122-4586-b261-9637ae62531d/thumbs/cover/800x800/png/5.png"
get gallery-4.png "$M/678c7548-5f68-477b-97c8-0399ef9907f4/images/12871567_9d78dece-53ee-4589-afb3-286240c009ed_2fe49727-51b5-4bd5-89ff-587a0676ce78/thumbs/cover/800x800/png/7.png"
get gallery-5.png "$M/b3f3fa4e-8902-4dcf-9fe3-f0a8102e38ff/images/12871576_817a6b31-7a33-4c89-98ad-cae152d4af11_0436a458-4e96-4f75-a951-44c12c529c66/thumbs/cover/800x800/png/printablesmakerworld-3d-tetris13.png"
get gallery-6.png "$M/b8091c02-d14a-47be-8a81-157bee4c68dd/images/12879409_fe6acd09-2355-4134-8afb-b58aa6b8da99_d539892e-6869-4ee5-a920-5692d6bed307/thumbs/cover/800x800/png/printablesmakerworld-3d-tetris14.png"

echo "Start-up and PCB"
get startup.gif          "$R/6471f96c-ef9c-423e-be6d-d03384f1a6a8/ezgif-1fe9434c10e6a155.gif"
get pcb-layout.webp      "$R/bf7c1e80-533a-4b16-98ec-0287ccdac4d5/thumbs/cover/800x430/png/grafik.webp"
get pcb-finished.jpg     "$R/d9fd2bd3-caa0-4415-8e51-3b0b377b94a1/snapchat-1579944203.jpg"
get print-parts.webp     "$R/2d8e0d9c-b6e3-4490-a2d6-ab9f2dc99650/thumbs/cover/800x522/png/grafik.webp"
get circuit-breadboard.webp "$R/4c7a785b-3725-4c42-9716-8c2b985e7ee6/thumbs/cover/800x491/png/besucherzahler_steckplatine.webp"
get circuit-soldered.webp   "$R/bed41b7f-5db2-426e-ad7b-97b9af69a2ea/thumbs/cover/800x600/jpg/img20260518082213.webp"

echo "Assembly"
get nuts-1.webp  "$R/e1319b73-3264-44d9-98e5-a16ffcfc070f/thumbs/cover/800x600/jpg/img20260518083123.webp"
get nuts-2.webp  "$R/4818c28e-98c7-4d04-925c-8ebdb12f7fc6/thumbs/cover/800x600/jpg/img20260518083135.webp"
get nuts-3.webp  "$R/63adc4bd-70fd-4b0e-8365-83d7dea1bb4a/thumbs/cover/800x625/png/grafik.webp"
get power-1.webp "$R/cba5ce18-7d5e-4222-a7ec-4fdb1de34d73/thumbs/cover/800x1067/jpg/img20260518082444.webp"
get power-2.webp "$R/bb4077df-7652-4679-a65d-def928a474c0/thumbs/cover/800x600/jpg/img20260518082420.webp"
get power-3.webp "$R/5c741d61-5182-4fc6-bb03-f74a6ccb670e/thumbs/cover/800x600/jpg/img20260518082753.webp"
get power-4.webp "$R/2f7fbcf2-3b5c-4226-948e-84fe0b686af8/thumbs/cover/800x600/jpg/img20260518082232.webp"
get power-5.webp "$R/b8fb6b67-fc5b-4100-bfcd-06f4f750eef3/thumbs/cover/800x600/jpg/img20260518082730.webp"
get top-1.webp   "$R/1e06a4dc-0a40-4228-bdec-560a1f820720/thumbs/cover/800x600/jpg/img20260518081841.webp"
get top-2.webp   "$R/6c9c59dc-35a2-46f5-a5db-03d4f0a8afaa/thumbs/cover/800x600/jpg/img20260518081918.webp"
get top-3.webp   "$R/661f64d4-c6b8-4bda-8fa3-d60493ad0590/thumbs/cover/800x600/jpg/img20260518081935.webp"
get mount-1.webp "$R/5fcb253c-4c6c-43c2-9d30-011cb32d5355/thumbs/cover/800x600/jpg/img20260518082339.webp"
get mount-2.webp "$R/e14f0afc-fbfe-429b-8d97-f83e4f8afa02/thumbs/cover/800x600/jpg/img20260518082129.webp"
get mount-3.webp "$R/c91190c4-3297-472b-b7b6-921cfbe2d9de/thumbs/cover/800x600/jpg/img20260518082037.webp"
get mount-4.webp "$R/932c7fcc-afeb-409f-9ccb-e00738a608aa/thumbs/cover/800x1067/jpg/img20260518082949.webp"
get close-1.webp "$R/872f0046-066c-459c-bb3e-196f5ed1a384/thumbs/cover/800x600/jpg/img20260518081717.webp"
get close-2.webp "$R/4da7861a-2deb-4b2f-8d39-517490433e71/thumbs/cover/800x600/jpg/img20260518083043.webp"

echo "Flowcharts"
get flow-setup.png "$R/64db24ec-0651-49c7-ab14-1f1fa971c548/setup_flow_en.png"
get flow-loop.png  "$R/71f55e8d-2d21-4caf-8758-aa1b3380ebb7/loop_event_handlers_en.png"

echo "Done. $(ls | wc -l) files in docs/images/"
