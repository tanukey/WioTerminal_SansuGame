# WioTerminal SansuGame

Wio Terminal (Seeed) 上で動く、幼稚園年長〜小学校低学年向けの算数(足し算・引き算)ゲームです。
MicroSDは使わず、問題はすべて起動中にランダム生成します。1ゲーム5問。

方針の詳細は [`user_documents/20260821_sansugame_housin.md`](user_documents/20260821_sansugame_housin.md) を参照してください。

## 遊び方

- タイトル画面で **Aボタン=かんたん(1桁)** / **Bボタン=ふつう(1〜2桁混在)** / **Cボタン=むずかしい(2桁同士)** を選ぶ。
- 出題画面では、画面の上下左右に4つの答えの候補が表示される。**5方向スイッチをその方向に倒す**とその選択肢を回答したことになる(確定操作は不要)。
- 正解は◯、不正解は×で一瞬フィードバックしたのち次の問題へ進む。
- 5問終わるとリザルト画面で正解数を表示。**Aボタンでもう一度**、**Cボタンでタイトルへ**戻る。
- 内蔵LCDフォントが日本語(ひらがな/漢字)グリフを持たないため、UIの文言はすべて記号・図形(◯×・難易度ドット・アイコン)で表現している。

## ビルド・書き込み

[PlatformIO](https://platformio.org/) を使用。

```bash
pio run                # ビルドのみ
pio run -t upload      # 通常の書き込み(環境によっては失敗する場合あり、下記参照)
```

### WSL2 + Windows接続時の書き込みについて

この開発環境(WSL2からWindows接続のWio Terminalへ書き込む構成)では、`usbipd`でUSBパススルーして
`pio run -t upload`(bossac/sam-ba経由)する方法は、Wio TerminalがUF2ブートローダーに入った際の
USB再接続でWSL側が不安定になり失敗しやすいことが分かっています。代わりに以下のUF2ドラッグ&ドロップ
方式を使ってください。

```bash
./tools/flash_uf2.sh
```

これで `pio run` によるビルド後、`firmware.bin` を `.uf2` に変換し(`tools/bin2uf2.py`)、
Windows側のデスクトップ(`/mnt/c/Users/<ユーザー名>/Desktop/`)にコピーします。
その後、手動で以下を行ってください。

1. Wio Terminal底面のリセットボタンをすばやく2回押し、UF2ブートローダーに入れる(オレンジLEDが点滅)。
2. Windowsのエクスプローラーに `Arduino` という名前のリムーバブルドライブが表示される。
3. コピーされた `sansugame_firmware.uf2` をそのドライブにドラッグ&ドロップする。

書き込みが終わると自動的に再起動してゲームが起動します。

`WIN_USER` 環境変数でコピー先のWindowsユーザー名を変更できます(未指定時は `tanukey`)。

## ソース構成

- `src/main.cpp` — ゲーム全体のステートマシン(タイトル→出題→フィードバック→リザルト)
- `src/question.{h,cpp}` — 難易度別の問題生成(演算子・オペランド範囲・4択のダミー選択肢生成)
- `src/ui.{h,cpp}` — 各画面の描画(TFT_eSPI互換の`Seeed_Arduino_LCD`を使用)
- `src/input.{h,cpp}` — A/B/Cボタンと5方向スイッチのデバウンス付き読み取り
- `tools/bin2uf2.py` / `tools/flash_uf2.sh` — 上記のUF2書き込みフロー用ツール
