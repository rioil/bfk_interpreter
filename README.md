# brainf*ckインタプリタ

brainf\*ck用のインタプリタです．
コマンドライン引数としてbrainf\*ckのソースファイルを指定することで，brainf\*ckプログラムを実行できます．
**なお，当インタプリタの使用は自己責任です．当インタプリタを使用したことにより発生したいかなる損害にも開発者は責任を負いません．**

## brainfu*kとは
  
このサイトがわかりやすかったです．
<http://www.kmonos.net/alang/etc/brainfuck.php>

## 当インタプリタの仕様

+ 配列の最大長：3000
+ ループ（ [ ] ）のネストの最大の深さ：100

  これらはソースコード（main.cpp）を少し書き換えることで簡単に変更できます．

  `#define BFK_ARRAY_SIZE この部分を変更`  
  `#define BFK_MAX_LOOP_DEPTH この部分を変更`

+ ポインタの範囲外参照チェック

  ポインタのインクリメント，デクリメント時に変更後のポインタが配列の範囲外を指すことがないようにチェックします．
  変更後に配列の範囲外を指すようになる場合は，そこでプログラムの実行を停止します．

## コンパイル方法

+ Windows

  makefileを使ってmakeコマンドでコンパイルすることができます．
  ただし，g++およびGNU makeを利用できる必要があります．

+ Linux

  通常のC++プログラムと同様にコンパイルしてください．

+ Mac

  手元にMacが無いため確認はできていませんが，通常のC++プログラムと同様にコンパイルできると思います．

---

## 連絡先

バグを見つけた場合には [Twitter](https://twitter.com/@rioil_dev) に連絡をして頂くか，Pull Requestを作成していただけると助かります．
その他，機能追加要望や質問があれば上記連絡先にお願いします．
