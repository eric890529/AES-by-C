/*
#include <stdio.h>
#include <stdint.h>
*/
/*
uint8_t s[4][4] = 
{
	{0x00, 0x04, 0x08, 0x0C},
	{0x01, 0x05, 0x09, 0x0D},
	{0x02, 0x06, 0x0A, 0x0E},
	{0x03, 0x07, 0x0B, 0x0F}
};*/


/*�ǤJ���block�|������ʧ��ܾ��block ���ݦ^�ǭ�*/ 
void shiftRow(uint8_t s[][4]){//�[�K�ഫ 
    uint8_t t;//�L�κA 
    /*�ĤG�C�������@��*/ 
    t=s[1][0];
    s[1][0]=s[1][1];
    s[1][1]=s[1][2];
    s[1][2]=s[1][3];
    s[1][3]=t;
    /*�ĤT�C�������@��*/ 
    t=s[2][0];
    s[2][0]=s[2][2];
    s[2][2]=t;
    t=s[2][1];
    s[2][1]=s[2][3];
    s[2][3]=t;
	/*�ĥ|�C�������@��*/ 
    t=s[3][0];
    s[3][0]=s[3][3];
    s[3][3]=s[3][2];
    s[3][2]=s[3][1];
    s[3][1]=t;
}//shiftRow

/*�ǤJ���block�|������ʧ��ܾ��block ���ݦ^�ǭ�*/  
void Inv_shiftRow(uint8_t s[][4]){//�ѱK�ഫ 
    uint8_t t;
    /*�ĤG�C���k���@��*/ 
    t=s[1][3];
    s[1][3]=s[1][2];
    s[1][2]=s[1][1];
    s[1][1]=s[1][0];
    s[1][0]=t;
    /*�ĤT���k�����*/ 
    t=s[2][0];
    s[2][0]=s[2][2];
    s[2][2]=t;
    t=s[2][1];
    s[2][1]=s[2][3];
    s[2][3]=t;
    /*�ĥ|�C���k���T��*/ 
    t=s[3][0];
    s[3][0]=s[3][1];
    s[3][1]=s[3][2];
    s[3][2]=s[3][3];
    s[3][3]=t;
}//Inv_shiftRow

/*
void sprint(){
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			printf("%x ",s[i][j]);
		}
		printf("\n");
	}
}
int main(){
	sprint();
	printf("\n");
	printf("�[�K�ഫ:\n");
	eshiftrow();
	sprint();
	printf("\n");
	printf("�ѱK�ഫ:\n");
	dshiftrow();
	sprint();
}*/
