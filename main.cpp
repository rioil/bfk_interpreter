#include <iostream>
using namespace std;

#define BFK_ARRAY_SIZE 3000
#define BFK_MAX_LOOP_DEPTH 100

int main(int argc, char *argv[]){
  if(argc == 1){
    cerr << "[error]���s����t�@�C�����w�肵�Ă��������D" << endl;
    return 0;
  }
  else if(argc > 2){
    cout << "[warning]�������������܂��D2�Ԗڈȍ~�̈����͖�������܂��D" << endl;
  }

  // �\�[�X�t�@�C���̑��݊m�F�C�I�[�v��
  FILE *bfk_source;
  if((bfk_source = fopen(argv[1], "r")) == NULL){
    cerr << "[error] " << argv[1] << " �͑��݂��܂���" << endl;
    return 0;
  }

  // brainfuck�Ŏg�p����z��C�|�C���^�̏���
  char bfk_array[BFK_ARRAY_SIZE] = {0,};
  const char *initial_bfk_pointer = bfk_array;
  char *bfk_pointer = bfk_array;

  // �\�[�X�R�[�h�̓ǂݍ��݁E���s�p�ϐ�
  char current_char;
  unsigned int program_counter = 0;
  unsigned int loop_depth = 0;
  unsigned int loop_start_position[BFK_MAX_LOOP_DEPTH] = {0,};

  // �\�[�X�R�[�h�����J�n
  while((current_char = fgetc(bfk_source)) != EOF){
    switch(current_char){
      case '>':
        if(bfk_pointer < initial_bfk_pointer+BFK_ARRAY_SIZE-1){
          bfk_pointer++;
        }
        else{
          cerr << "[error]�z��̍ő�͈͂𒴂��܂����D�v���O�����̎��s���~���܂��D" ;
          return 1;
        }
      break;

      case '<':
        if(bfk_pointer > initial_bfk_pointer){
          bfk_pointer--;
        }
        else{
          cerr << "[error]�z��̍ŏ��͈͂𒴂��܂����D�v���O�����̎��s���~���܂��D" ;
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
        // ���݂̏ꏊ���L�^�E���[�v�̐[�����X�V
        loop_start_position[loop_depth] = program_counter;
        loop_depth++;

        // ����]�܂Ői��
        if(*bfk_pointer == 0){
          while((current_char = fgetc(bfk_source)) != ']'){
            if(current_char == EOF){
              cerr << "[error] " << program_counter + 1 << " �����ڂ�[����C����]�ɐi�ނ̂Ɏ��s���܂����D" ;
              return 1;
            }
            program_counter++;
          }
          // ]��ǂݍ��񂾕��C������program_counter���C���N�������g����K�v������
          // switch���program_counter++��[��ǂݍ��񂾕��̏���
          program_counter++;
          loop_depth--;
        }
      break;

      case ']':
        // �O��[�܂Ŗ߂�C���[�v�̐[�����X�V
        if(loop_depth == 0){
          cerr << "[error] " << program_counter + 1 << " �����ڂ�]���O��[�͑��݂��܂���" ;
          return 1;
        }
        program_counter = loop_start_position[loop_depth-1];
        if(fseek(bfk_source, program_counter, SEEK_SET)){
          cerr << "[error] " << program_counter + 1 << " �����ڂ�]����C1�O��[�ɖ߂�̂Ɏ��s���܂����D" ;
          return 1;
        }
        loop_depth--;
      continue;

      default:
      continue;
    }
    program_counter++;
  }

  // ����I���ʒm
  cout << "�v���O�����͐���ɏI�����܂����D" << endl;
  return 0;
}