#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>

int static control = 0;
#define  champion 17
int changeNumber()
{
    if(control == 0)
    {
      control = control + 2;
      return 84;
    }
    else if(control == 1)
    {
      return 69;
    }
    else if(control == 2)
    {
      return 82;
    }
}


void* firstWord(void *arg) {
	  char misteriousOne = 79;
    printf("%c ", misteriousOne);

}

void* secondWord(void *arg) {
    char misteriousTwo;
    misteriousTwo = changeNumber();
    printf("%c", misteriousTwo);
    control = control - 1;
    misteriousTwo = changeNumber();
    printf("%c", misteriousTwo);
    control = control - 1;
    misteriousTwo = changeNumber();
    printf("%c", misteriousTwo);
    misteriousTwo = changeNumber();
    printf("%c", misteriousTwo);
    misteriousTwo = misteriousTwo - champion;
    printf("%c ", misteriousTwo);

}



void* thirdWord(void *arg) {
	  char misteriousThree = 86;
    printf("%c", misteriousThree);
    misteriousThree = misteriousThree - champion;
    printf("%c", misteriousThree);
    misteriousThree = misteriousThree + 8;
    printf("%c \n", misteriousThree);


}


int main(int argc, char *argv[]) {

	int th1, th2, th3;
  int i;

  th1 = ccreate(firstWord, (void *)&i, 0);
  th2 = ccreate(secondWord, (void *)&i, 0);
  th3 = ccreate(thirdWord, (void *)&i, 0);


  cjoin(th1);
  cjoin(th2);
  cjoin(th3);
}
