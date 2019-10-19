#include <iostream>
using namespace std;

#define BFK_ARRAY_SIZE 3000
#define BFK_MAX_LOOP_DEPTH 100

int main(int argc, char *argv[]){
  if(argc == 1){
    cerr << "[error]���s����t�@�C�����w�肵�Ă��������D" << endl;
    return 1;
  }
  else if(argc > 2){
    cout << "[warning]�������������܂��D2�Ԗڈȍ~�̈����͖�������܂��D" << endl;
  }

  // �\�[�X�t�@�C���̑��݊m�F�C�I�[�v��
  FILE *bfk_source;
  if((bfk_source = fopen(argv[1], "r")) == NULL){
    cerr << "[error] " << argv[1] << " �͑��݂��܂���" << endl;
    return 1;
  }

  // brainfuck�Ŏg�p����z��C�|�C���^�̏���
  char bfk_array[BFK_ARRAY_SIZE] = {0,};
  const char *initial_bfk_pointer = bfk_array;
  char *bfk_pointer = bfk_array;

  // �\�[�X�R�[�h�̓ǂݍ��݁E���s�p�ϐ�
  char current_char;
  unsigned int loop_depth = 0;
  fpos_t previous_pos;
  fgetpos(bfk_source, &previous_pos); // init
  fpos_t loop_start_pos[BFK_MAX_LOOP_DEPTH] = {0,};
  fpos_t loop_end_pos[BFK_MAX_LOOP_DEPTH] = {-1,};
  bool jumped = false;
  unsigned long long program_step=1;
  unsigned int inside_loop = 0;

  // �\�[�X�R�[�h�����J�n
  while((current_char = fgetc(bfk_source)) != EOF){
    switch(current_char){
      case '>':
        if(bfk_pointer < initial_bfk_pointer+BFK_ARRAY_SIZE-1){
          bfk_pointer++;
        }
        else{
          cerr << "[error] " << program_step + 1 << " �X�e�b�v�ڂ�>�Ŕz��̍ő�͈͂𒴂��܂����D�v���O�����̎��s���~���܂��D" ;
          return 1;
        }
      break;

      case '<':
        if(bfk_pointer > initial_bfk_pointer){
          bfk_pointer--;
        }
        else{
          cerr << "[error] " << program_step + 1 << " �X�e�b�v�ڂ�<�Ŕz��̍ŏ��͈͂𒴂��܂����D�v���O�����̎��s���~���܂��D" << program_step ;
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
        cout << "�������l����͂��Ă��������F";
        cin >> *bfk_pointer;
      break;

      case '[':
        // ���[�v�̐[�����E���`�F�b�N
        if(loop_depth == BFK_MAX_LOOP_DEPTH){
          cerr << "[error] " << program_step + 1 << " �X�e�b�v�ڂ�[�Ń��[�v�̃l�X�g�̐[���̍ő�𒴂��܂����D�v���O�����̎��s���~���܂��D" ;
          return 1;
        }

        // ���[�v�J�n�ʒu�̋L�^
        if(!jumped){
          loop_start_pos[loop_depth] = previous_pos;
        }
        jumped = false;

        // �|�C���^��0�Ȃ�Ή�����]�܂Ői��
        if(*bfk_pointer == 0){
          inside_loop = 0;
          while(1){
            current_char = fgetc(bfk_source);
            switch(current_char){
              case EOF:
                cerr << "[error] " << program_step + 1 << " �X�e�b�v�ڂ�[�ɑΉ�����]�����݂��܂���D�v���O�����̎��s���~���܂��D" ;
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

        // ���[�v�̐[�����X�V
        loop_depth++;
      break;

      case ']':
        // �O��[�܂Ŗ߂�C���[�v�̐[�����X�V
        if(loop_depth == 0){
          cerr << "[error] " << program_step + 1 << " �X�e�b�v�ڂ�]���O��[�͑��݂��܂���D�v���O�����̎��s���~���܂��D" ;
          return 1;
        }
        //�@���[�v�J�n�ʒu�փW�����v
        fsetpos(bfk_source, &loop_start_pos[loop_depth-1]);
        jumped = true;
        loop_depth--;
      break;

      default:
        // ���̕����̓R�����g�Ƃ��ēǂݔ�΂�
        program_step--;
      break;
    }
    // ���݂̈ʒu���L��
    fgetpos(bfk_source, &previous_pos);
    program_step++;
  }

  // �I�������E����I���ʒm
  fclose(bfk_source);
  cout << "\n�v���O�����͐���ɏI�����܂����D" << endl;
  return 0;
}