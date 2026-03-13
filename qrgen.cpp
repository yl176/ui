#include "qrgen.h"

using namespace std;  

QqrWidget::QqrWidget()
{
	//setMaximumSize(137, 184);
	//setMinimumSize(137, 184);
	setMaximumSize(320, 320);
	setMinimumSize(320, 320);
	//setStyleSheet("background:white");
	setStyleSheet("QWidget{background-color:rgb(255, 255, 255);"\
		                           "border-radius:5px;}");

	QWidget *background_wid = new QWidget(this);
	background_wid->setFixedSize(157, 204);
	background_wid->setStyleSheet("QWidget{background-color:rgb(255, 255, 255);"\
										   "border-radius:5px;}");
	background_wid->move(81, 58);

    QWidget *back_wid = new QWidget(background_wid);
	back_wid->setFixedSize(137, 184);
	back_wid->setStyleSheet("QWidget{background-color:rgb(255, 255, 255);"\
		                    "border-width: 5px; border-style: solid;"\
						    "border-color: black; border-radius: 10px;}");
    back_wid->raise();
	back_wid->move(10,10);
	
	pair_label = new QLabel("123-45-678", back_wid);



	pair_label->setStyleSheet("font-size:20px;letter-spacing:6px;background-color:white;"\
	                          "border-style:none;color:black;"\
	                          "font-weight:normal;text-align:left;");

	
    pair_label->setText("111-22-333");
	pair_label->setGeometry(QRect(51, 12, 76, 44));	

    tool1_button = new QToolButton(back_wid);
	tool1_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	QPixmap button_pixmap("hkt");
	tool1_button->setIcon(button_pixmap);
	tool1_button->setIconSize(button_pixmap.size());
	//tool1_button->setFixedSize(button_pixmap.width()+0, button_pixmap.height());
	tool1_button->setFixedSize(35, 44);
	tool1_button->setGeometry(QRect(9, 12, 35, 44));
	tool1_button->setCheckable(false);
	tool1_button->setStyleSheet("font-size:20px;background-color:white;"\
	                          "border-style:none;color:black;"\
	                          "font-weight:normal;text-align:left;");

	tool_button_qr = new QToolButton(back_wid);
	tool_button_qr->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	//mask by iovine 
	//QPixmap button_pixmap_qr("qr_png");
	//tool_button_qr->setIcon(button_pixmap_qr);
	//tool_button_qr->setIconSize(button_pixmap_qr.size());
	
	tool_button_qr->setFixedSize(109, 109);
	
	tool_button_qr->setGeometry(QRect(9, 12+44+9, 109, 109));
	tool_button_qr->setCheckable(false);
	tool_button_qr->setStyleSheet("font-size:35px;background-color:white;"\
	                          "border-style:none;color:black;"\
	                          "font-weight:normal;text-align:left;");
	connect(tool_button_qr,SIGNAL(clicked()),this,SLOT(qr_clicked()));

	memset(setup_id, 0, SETUP_ID_LEN);
	memset(setup_code, 0, SETUP_CODE_LEN);

	memset(g_init_uri,0,MAX_LEN_URI);


}
void QqrWidget::mousePressEvent(QMouseEvent *event)
{
    //qDebug()<<"QqrWidget,mousePressEvent, signals, clearPairCode";
    if(qrFlag == qr_pair)
    {
        emit clearPairCode();
    }    
}


void QqrWidget::qr_clicked(void)
{
    if(qrFlag == qr_pair)
    {
       emit clearPairCode(); 
    }    
}


void QqrWidget::GetSetupId(char *str)
{
    if (NULL == str)
    {
        qDebug()<<"[error]GetSetupId str NULL";
    }
    memset(setup_id, 0, SETUP_ID_LEN);
	memcpy(setup_id, str, SETUP_ID_LEN);
	//qDebug()<<"";
	
	qDebug()<<"GetSetupid=["<<setup_id<<"]";
}
void QqrWidget::GetSetupCode(char *str)
{
    char *src = str;
	char *dest = setup_code;
    if (NULL == str)
    {
        qDebug()<<"[error]GetSetupCode str NULL";
    }

    if (strlen(str) > SETUP_CODE_LEN + 2)
    {
	   // qDebug()<<"[error]GetSetupCode strlen="<<strlen(str);
    }
	
    memset(setup_code, 0, SETUP_CODE_LEN);
	while(*str)
	{
	    if(*str != '-')
    	{
			*dest++ = *str++;
    	}
		else
		{
		    str++;
		}
	}
	
	//qDebug()<<"GetSetupCode=["<<setup_code<<"]";
}

char QqrWidget::getStrFromInt(int value)  
{  
    if(value >= 0 && value <= 9)  
        return (value + '0');  
    if(value >= 10 && value <= 35)  
        return ('A' + (value-10));  
    else  
        qDebug() << "error!!!" << endl;  
    return '#';  
}  
  
int QqrWidget::getIntFromStr(char value)  
{  
    if(value >= '0' && value <= '9')  
        return value - '0';  
    else if(value >= 'A' && value <= 'Z')  
        return (value - 'A') + 10;  
}  
  
char QqrWidget::qiuyu(string &num, int M, int N)  
{  
    int remain = 0;  
    string shang;  
  
    for(int i=0; i<num.size(); i++)  
    {  
        remain = remain*M + getIntFromStr(num[i]);  
        shang += getStrFromInt(remain / N);  
        remain = (remain % N);   
    }  
    while(shang.size() > 0 && shang[0] == '0')  
    {  
        shang = shang.substr(1, shang.size()-1);  
    }  
    num = shang;  
    return getStrFromInt(remain);  
}  
  
string QqrWidget::baseConvert(string num, int M, int N)  
{  
    string result;  
    string shang = num;  
    while(shang.size() > 0)  
    {  
        result += qiuyu(shang, M, N);  
    }  
    std::reverse(result.begin(), result.end());  
    return result;  
} 


void QqrWidget::DToB(long n,int len) 
{
	static int k=0;
	k++;
	if(k<len)DToB(n/2,len); 
	qDebug()<<n%2;
}

#define MAX_T 33  
/* 十进制数转换为二进制数（字符串表示） */
char *QqrWidget::decToBin(long num,int* len)
{
    char *arr;
    char temp;
    int i, n;
 
    arr = (char *)malloc(sizeof(char) * MAX_T);
    n = 0;
    while(num > 0)
    {
        arr[n++] = num % 2 + '0';
        num /= 2;
    }
 
    for(i=0; i<n/2; i++)
    {
        temp = arr[i];
        arr[i] = arr[n-1-i];
        arr[n-1-i] = temp;
    }
    arr[n] = '\0';
	*len=n;
 
    return arr;
}

//char g_setup_code_buf[8];
int QqrWidget::make_rand_setup_code(long* setupcode)
{
	int rand_buf[8];
	int numb=0,i=0;
	long pin=0;
	srand(time(NULL)); //用当前系统时间设置种子
	for(i=0;i<8;i++)
	{
		numb=rand()%10;
		if(numb<10)
		{
			rand_buf[i]=numb;
		}	
		else
		{
			rand_buf[i]=0;
		}

	}
	if(rand_buf[0]==0)
	{
		rand_buf[0]=5;
	}
	for(i=0;i<8;i++)
	{
		pin+=rand_buf[i]*pow(10,(7-i));
	}

	*setupcode=pin;	
	//sprintf(g_setup_code_buf,"%ld",pin);
	
	
	//printf("pin=%ld \r\n",pin);
	
	return 0;

	
}

//ofstream oFile_backup; 

#define MUNB_ALL 100

#define CUSTOMER_CODE 0001 //max 255
#define DEVICE_TYPE   03   //Ipcam 
#define SUBTYPE       02	//enno360Cam 
#define YEAR          18
#define WEEKEND       16

#define CATEGORY 17 //for IPcam

void QqrWidget::doflutterQrgen() 
{


	qDebug()<<"flutter setup_id="<<setup_id<<endl;

	pair_label->setText(setup_id);
		 
	 tool_button_qr->update();
	 tool_button_qr->setEnabled(true);
	
	 qrFlag = qr_pair;


}


void QqrWidget::doQrgen()  
{  
	int retval = 0;
    int numb = 1;

    string bin = "0000000000001111010011000101101000100101000110";  
	char *qr_start= "X-HM://";
	char *config_bin = "1010";
	char categoryandsetupcode_bin[64];//46
	char qrcmd[128];
	
	char pin_buf[64];//27
	char category_buf[8];
	char* pin_buf_t=NULL;
	char *category_buf_t=NULL;
	char data_buf[]="0123456789";
	char char_buf[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	char categoryandsetupcode[32];//9
	int data_len=0;
	int i=0,j=0;
	int tmp=0,char_id=0;
	int pin_len=0,category_len=0;
	char setup_ID[4];
	char setup_payload[24];//13
	char setup_code_tmp[8];
	char QR_setup_payload[20];
	char setup_code_disp[SETUP_CODE_LEN+1];

    //qDebug()<<"doQrgen";
	if (strlen(setup_id) != SETUP_ID_LEN-1)
	{
	    qDebug()<<"[qterror]doQrgen setup_id=["<<setup_id<<"]";
		return;
	}
	
	if (strlen(setup_code) != SETUP_CODE_LEN-1)
	{
	    qDebug()<<"[qterror]doQrgen setup_code=["<<setup_code<<"]";
		return;
	}

	for(j=0;j<7;j++)
	categoryandsetupcode_bin[j]='0';		
		
	long category=CATEGORY;
	category_buf_t=decToBin(category,&category_len);
	
	
	
	//printf("category_len=%d\r\n",category_len);
	if(category_len<8)
	{
		i=8-category_len;
		for(j=0;j<i;j++)
		category_buf[j]='0';	
	}
	else
	{
		j=0;
	}

	sprintf(&category_buf[j],"%s",category_buf_t);
	
	//qDebug()<<"category_buf="<<category_buf<<endl;

	sprintf(&categoryandsetupcode_bin[7],"%s",category_buf);
	sprintf(&categoryandsetupcode_bin[15],"%s",config_bin);
	
	long pin=0;//51808582;
//	make_rand_setup_code(&pin);


	qDebug()<<"setup_code="<<setup_code<<endl;



	
	pin = atoi(setup_code);
	
	pin_buf_t=decToBin(pin,&pin_len);


	
	qDebug()<<"pin_buf_t="<<pin_buf_t<<endl;
	
	//printf("pin_len=%d\r\n",pin_len);
	if(pin_len<27)
	{
		i=27-pin_len;
		for(j=0;j<i;j++)
		pin_buf[j]='0';	
	}
	else
	{
		j=0;
	}



	snprintf(&pin_buf[j],27,"%s",pin_buf_t);
	
	qDebug()<<pin_buf<<endl;

	snprintf(&categoryandsetupcode_bin[19],27,"%s",pin_buf);
	
	qDebug()<<categoryandsetupcode_bin<<endl;

	//qDebug()<<bin<<endl;




	//
	
    string b36 = baseConvert(categoryandsetupcode_bin, 2, 36);  
	
	data_len=b36.size();
	if(data_len<9)
	{
		//qDebug()<< b36.size() << endl;
		i=9-data_len;
		for(j=0;j<i;j++)
		categoryandsetupcode[j]='0';
	}
	else
	{
		j=0;
	}
	char b36_buf[9];
	strcpy(b36_buf,b36.c_str());
	
	snprintf(&categoryandsetupcode[j],9,"%s",b36_buf);
	
    qDebug() << categoryandsetupcode << endl;  
	
	// string b2= baseConvert(categoryandsetupcode, 36, 2);  
	// qDebug() << b2 << endl; 

	//
	
	
	int rand_buf[3];
	srand(time(NULL)); //用当前系统时间设置种子
	for(i=0;i<3;i++)
	rand_buf[i]=(rand()%10);    //用rand函数生成0-9的随机数，并赋值给数组rand_buf[i]

	snprintf(setup_payload,13,"%s",categoryandsetupcode);
	snprintf(&setup_payload[9],6,"%s",setup_id);	
	
	//qDebug() << "setup_payload="<<setup_payload << endl; 
	
	strcpy(QR_setup_payload,qr_start);
	strcat(QR_setup_payload,setup_payload);
	//qDebug() << "QR_setup_payload=["<<QR_setup_payload <<"]"; 


	memcpy(setup_code_disp, setup_code, 4);
	setup_code_disp[4] = '\n';
	memcpy(&setup_code_disp[5], &setup_code[4], 5);

	qDebug()<<"setup_code="<<setup_code_disp<<endl;




    pair_label->setText(setup_code_disp);




	//mask by iovine 
    //snprintf(qrcmd, 128, "qrencode -s 5 -m 2 %s -o qr_png", QR_setup_payload);
	//qDebug()<<"cmd=["<<qrcmd<<"]";
	//system(qrcmd);
 
	

	//mask by iovine 
	//QPixmap button_pixmap_qr("qr_png");
	//tool_button_qr->setIcon(button_pixmap_qr);
	//tool_button_qr->setIconSize(button_pixmap_qr.size());

	
	tool_button_qr->update();
	tool_button_qr->setEnabled(true);

	qrFlag = qr_pair;





}  
#if 0
int main()
{
	int numb = MUNB_ALL;
	int i=0;
	time_t t = time(0); 
	char name_backup[64]; 
	strftime( name_backup, sizeof(name_backup), "enno360Cam_QR_Str_%Y%m%d%H%M%S.csv",localtime(&t) );
	
	//oFile.open("enno360Cam_QR_Str_%s.csv",ios::out | ios::trunc);  
	oFile_backup.open(name_backup,ios::out | ios::trunc); 	
    oFile_backup << "No." << "," <<"enno360Cam id"<< "," << "Setup Code" << "," << "QR1 for homekit" << "," << "QR2 for enno config" << endl;  
   
	
	for (i=1;i<numb+1;i++)
	{
		work(i);
		sleep(1);
	}
	oFile_backup.close();	
}

#endif
char g_init_uri[MAX_LEN_URI];

void QqrWidget::doQrgenInit(void)
{
	char qrcmd[MAX_LEN_URI];   

	memset(qrcmd,0,MAX_LEN_URI);
	memcpy(g_init_uri,INIT_URI,strlen(INIT_URI));

	qDebug() << "######doQrgenInit##########"<<"]"; 

	if(strlen(g_init_uri)==0)
	{
	    snprintf(qrcmd, 128, "qrencode -s 5 -m 2 %s -o qr_init_png", INIT_URI); 
	}
	else
	{
	    snprintf(qrcmd, 128, "qrencode -s 5 -m 2 %s -o qr_init_png", g_init_uri);
	}	

	system(qrcmd);
	//mask by iovine 
	//QPixmap button_pixmap_qr("qr_init_png");
	//tool_button_qr->setIcon(button_pixmap_qr);
	//tool_button_qr->setIconSize(button_pixmap_qr.size());
	tool_button_qr->update();
	tool_button_qr->setDisabled(true);
	pair_label->setText("");
	qrFlag = qr_lock;
}

StartWordWidget::StartWordWidget()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
	this->setMaximumSize(OLED_PIX_X, OLED_PIX_Y);
	this->setMinimumSize(OLED_PIX_X, OLED_PIX_Y);

	wordFirstLine = new QLabel(this);
	wordFirstLine->setText(tr("scan the QR code"));
	wordFirstLine->setStyleSheet("font-size:30px;color:white;");
	//no_curtain->setGeometry(QRect(0, 0, 320, 120));
	wordFirstLine->setGeometry(QRect(0, 0, OLED_PIX_X, 60)); 
	wordFirstLine->setWordWrap(true); 
	wordFirstLine->setAlignment(Qt::AlignCenter); 

	
	wordSecondLine = new QLabel(this);
	wordSecondLine->setText(tr("enjoy immediately"));
	wordSecondLine->setStyleSheet("font-size:30px;color:white;");
	//no_curtain->setGeometry(QRect(0, 0, 320, 120));
	wordSecondLine->setGeometry(QRect(0, 80, OLED_PIX_X, 60)); 
	wordSecondLine->setWordWrap(true); 
	wordSecondLine->setAlignment(Qt::AlignCenter); 

	wordThirdLine = new QLabel(this);
	wordThirdLine->setText(tr("whole house AI control system"));
	wordThirdLine->setStyleSheet("font-size:30px;color:white;");
	//no_curtain->setGeometry(QRect(0, 0, 320, 120));
	wordThirdLine->setGeometry(QRect(0, 160, OLED_PIX_X, 60)); 
	wordThirdLine->setWordWrap(true); 
	wordThirdLine->setAlignment(Qt::AlignCenter); 
}

StartWordWidget::~StartWordWidget()
{
    if(wordFirstLine)
    {
        delete wordFirstLine;
		wordFirstLine = NULL;
    }

	if(wordSecondLine)
    {
        delete wordSecondLine;
		wordSecondLine = NULL;
    }

	if(wordThirdLine)
    {
        delete wordThirdLine;
		wordThirdLine = NULL;
    }
}

void StartWordWidget::translateLanguage(void)
{
	this->setText();
}

void StartWordWidget::setTextType(int textType)
{
    m_enTextType = textType;
}


void StartWordWidget::setText(void)
{
    if(m_enTextType == lock_text)
    {
       wordFirstLine->setText(tr("scan the QR code"));
	   wordSecondLine->setText(tr("enjoy immediately"));
	   wordThirdLine->setText(tr("whole house AI control system"));
    }
	else if(m_enTextType == pair_text)
	{
	    wordFirstLine->setText(tr("scan the QR code"));
	    wordSecondLine->setText(tr("make pairing"));
	    wordThirdLine->setText(tr(""));
	}
}

