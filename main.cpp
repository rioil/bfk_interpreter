#include <iostream>
using namespace std;

#define BFK_ARRAY_SIZE 3000
#define BFK_MAX_LOOP_DEPTH 100

int main(int argc, char *argv[]){
  if(argc == 1){
    cerr << "[error]実行するファイルを指定してください．" << endl;
    return 1;
  }
  else if(argc > 2){
    cout << "[warning]引数が多すぎます．2番目以降の引数は無視されます．" << endl;
  }

  // ソースファイルの存在確認，オープン
  FILE *bfk_source;
  if((bfk_source = fopen(argv[1], "r")) == NULL){
    cerr << "[error] " << argv[1] << " は存在しません" << endl;
    return 1;
  }

  // brainfuckで使用する配列，ポインタの準備
  char bfk_array[BFK_ARRAY_SIZE] = {0,};
  const char *initial_bfk_pointer = bfk_array;
  char *bfk_pointer = bfk_array;

  // ソースコードの読み込み・実行用変数
  char current_char;
  unsigned int loop_depth = 0;
  fpos_t previous_pos;
  fgetpos(bfk_source, &previous_pos); // init
  fpos_t loop_start_pos[BFK_MAX_LOOP_DEPTH] = {0,};
  fpos_t loop_end_pos[BFK_MAX_LOOP_DEPTH] = {-1,};
  bool jumped = false;
  unsigned long long program_step=1;
  unsigned int inside_loop = 0;

  // ソースコード処理開始
  while((current_char = fgetc(bfk_source)) != EOF){
    switch(current_char){
      case '>':
        if(bfk_pointer < initial_bfk_pointer+BFK_ARRAY_SIZE-1){
          bfk_pointer++;
        }
        else{
          cerr << "[error] " << program_step + 1 << " ステップ目の>で配列の最大範囲を超えました．プログラムの実行を停止します．" ;
          return 1;
        }
      break;

      case '<':
        if(bfk_pointer > initial_bfk_pointer){
          bfk_pointer--;
        }
        else{
          cerr << "[error] " << program_step + 1 << " ステップ目の<で配列の最小範囲を超えました．プログラムの実行を停止します．" << program_step ;
          return 1;
        }
      break;

      case '+':
        (*bfk_pointer)++;
      break;

      case '-':
        (*bfk_pointer)--;
      break;

      case '.':
        cout << *bfk_pointer << flush;
      break;

      case ',':
        cout << "代入する値を入力してください：";
        cin >> *bfk_pointer;
      break;

      case '[':
        // ループの深さ限界をチェック
        if(loop_depth == BFK_MAX_LOOP_DEPTH){
          cerr << "[error] " << program_step + 1 << " ステップ目の[でループのネストの深さの最大を超えました．プログラムの実行を停止します．" ;
          return 1;
        }

        // ループ開始位置の記録
        if(!jumped){
          loop_start_pos[loop_depth] = previous_pos;
        }
        jumped = false;

        // ポインタが0なら対応する]まで進む
        if(*bfk_pointer == 0){
          inside_loop = 0;
          while(1){
            current_char = fgetc(bfk_source);
            switch(current_char){
              case EOF:
                cerr << "[error] " << program_step + 1 << " ステップ目の[に対応する]が存在しません．プログラムの実行を停止します．" ;
              return 1;

              case ']':
                if(inside_loop == 0){
                  goto FINISH_PROCESSING_LOOP;
                }
                inside_loop--;
              break;

              case '[':
                inside_loop++;
              break;
            }
          }
          FINISH_PROCESSING_LOOP:
          break;
        }

        // ループの深さを更新
        loop_depth++;
      break;

      case ']':
        // 前の[まで戻り，ループの深さを更新
        if(loop_depth == 0){
          cerr << "[error] " << program_step + 1 << " ステップ目の]より前に[は存在しません．プログラムの実行を停止します．" ;
          return 1;
        }
        //　ループ開始位置へジャンプ
        fsetpos(bfk_source, &loop_start_pos[loop_depth-1]);
        jumped = true;
        loop_depth--;
      break;

      default:
        // 他の文字はコメントとして読み飛ばす
        program_step--;
      break;
    }
    // 現在の位置を記憶
    fgetpos(bfk_source, &previous_pos);
    program_step++;
  }

  // 終了処理・正常終了通知
  fclose(bfk_source);
  cout << "\nプログラムは正常に終了しました．" << endl;
  return 0;
}