#include <iostream>
using namespace std;

#define BFK_ARRAY_SIZE 3000
#define BFK_MAX_LOOP_DEPTH 100

int main(int argc, char *argv[]){
  if(argc == 1){
    cerr << "[error]実行するファイルを指定してください．" << endl;
    return 0;
  }
  else if(argc > 2){
    cout << "[warning]引数が多すぎます．2番目以降の引数は無視されます．" << endl;
  }

  // ソースファイルの存在確認，オープン
  FILE *bfk_source;
  if((bfk_source = fopen(argv[1], "r")) == NULL){
    cerr << "[error] " << argv[1] << " は存在しません" << endl;
    return 0;
  }

  // brainfuckで使用する配列，ポインタの準備
  char bfk_array[BFK_ARRAY_SIZE] = {0,};
  const char *initial_bfk_pointer = bfk_array;
  char *bfk_pointer = bfk_array;

  // ソースコードの読み込み・実行用変数
  char current_char;
  unsigned int program_counter = 0;
  unsigned int loop_depth = 0;
  unsigned int loop_start_position[BFK_MAX_LOOP_DEPTH] = {0,};

  // ソースコード処理開始
  while((current_char = fgetc(bfk_source)) != EOF){
    switch(current_char){
      case '>':
        if(bfk_pointer < initial_bfk_pointer+BFK_ARRAY_SIZE-1){
          bfk_pointer++;
        }
        else{
          cerr << "[error]配列の最大範囲を超えました．プログラムの実行を停止します．" ;
          return 1;
        }
      break;

      case '<':
        if(bfk_pointer > initial_bfk_pointer){
          bfk_pointer--;
        }
        else{
          cerr << "[error]配列の最小範囲を超えました．プログラムの実行を停止します．" ;
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
        // 現在の場所を記録・ループの深さを更新
        loop_start_position[loop_depth] = program_counter;
        loop_depth++;

        // 次の]まで進む
        if(*bfk_pointer == 0){
          while((current_char = fgetc(bfk_source)) != ']'){
            if(current_char == EOF){
              cerr << "[error] " << program_counter + 1 << " 文字目の[から，次の]に進むのに失敗しました．" ;
              return 1;
            }
            program_counter++;
          }
          // ]を読み込んだ分，ここでprogram_counterをインクリメントする必要がある
          // switch後のprogram_counter++は[を読み込んだ分の処理
          program_counter++;
          loop_depth--;
        }
      break;

      case ']':
        // 前の[まで戻り，ループの深さを更新
        if(loop_depth == 0){
          cerr << "[error] " << program_counter + 1 << " 文字目の]より前に[は存在しません" ;
          return 1;
        }
        program_counter = loop_start_position[loop_depth-1];
        if(fseek(bfk_source, program_counter, SEEK_SET)){
          cerr << "[error] " << program_counter + 1 << " 文字目の]から，1つ前の[に戻るのに失敗しました．" ;
          return 1;
        }
        loop_depth--;
      continue;

      default:
      continue;
    }
    program_counter++;
  }

  // 正常終了通知
  cout << "プログラムは正常に終了しました．" << endl;
  return 0;
}