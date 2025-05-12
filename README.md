# KEROKERO Voice

ケロケロします

## なぜ?

なんとなく

## ファームウェア準備

普通にPlatformIOでコンパイルと書き込みしてね。  
初回だけヘッダファイル作成で時間がかかります。

## 使い方

ボタンAを押すと組み込まれた文字列をケロケロします

シリアルポート(115200bps)から入力した文字列でもケロケロします。

## 仕様

ひらがなとカタカナのみ正しく認識します。  
それ以外の文字は適当な音をランダムで喋ります。

### 音声について

このライブラリには、HTS Voice "Mei" を用いて合成した音声データが含まれています。  
音声の合成には OpenJTalk を使用しました。

HTS Voice "Mei"  
released by MMDAgent Project Team  
http://www.mmdagent.jp/  
Copyright (c) 2009–2018  Nagoya Institute of Technology Department of Computer Science  

Licensed under the Creative Commons Attribution 3.0 license.  
See: https://creativecommons.org/licenses/by/3.0/ for datails
