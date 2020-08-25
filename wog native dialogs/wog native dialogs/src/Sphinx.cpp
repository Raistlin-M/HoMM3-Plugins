/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Диалог IF:D/E /////////////////////////////////////////////////////////

int BanDlg_CustomReq_EnterText = false;

// #define EXPORT comment(linker, "/EXPORT:" UseWin32InputControl"="d_UseWin32InputControl)
extern "C" __declspec(dllexport) int UseWin32InputControl(int newState);

int UseWin32InputControl(int newState)
{
	// #pragma EXPORT
	// читаем предыдущее состояние
	int prevState = BanDlg_CustomReq_EnterText;

	if ( newState ) {
		BanDlg_CustomReq_EnterText = true;
	} else BanDlg_CustomReq_EnterText = false;

	// возвращаем предыдущее состояние
	return prevState;
}




#define OPTION_ID 911

_Sphinx1_* o_Sphinx1 = 0;

_bool_ Dlg_CustomReq_Ban = false;


bool IsSupportedFormatImage(char* image_name) 
{
	bool ret = false;
	char im_name[256];
	MemCopy(im_name, image_name, 256);		
	int len = strlen(im_name);
	im_name[len] = 0;

	// bmp 
	if ( im_name[len-4] == '.' && im_name[len-3] == 'b' && im_name[len-2] == 'm' && im_name[len-1] == 'p' )
		ret = true;
	if ( im_name[len-4] == '.' && im_name[len-3] == 'B' && im_name[len-2] == 'M' && im_name[len-1] == 'P' )
		ret = true;
	// pcx 
	if ( im_name[len-4] == '.' && im_name[len-3] == 'p' && im_name[len-2] == 'c' && im_name[len-1] == 'x' )
		ret = true;
	if ( im_name[len-4] == '.' && im_name[len-3] == 'P' && im_name[len-2] == 'C' && im_name[len-1] == 'X' )
		ret = true;
	// jpg 
	if ( im_name[len-4] == '.' && im_name[len-3] == 'j' && im_name[len-2] == 'p' && im_name[len-1] == 'g' )
		ret = true;
	if ( im_name[len-4] == '.' && im_name[len-3] == 'J' && im_name[len-2] == 'P' && im_name[len-1] == 'G' )
		ret = true;
	// jpeg 
	if ( im_name[len-5] == '.' && im_name[len-4] == 'j' && im_name[len-3] == 'p' && im_name[len-2] == 'e' && im_name[len-1] == 'g' )
		ret = true;
	if ( im_name[len-5] == '.' && im_name[len-4] == 'J' && im_name[len-3] == 'P' && im_name[len-2] == 'E' && im_name[len-1] == 'G' )
		ret = true;
	// png 
	if ( im_name[len-4] == '.' && im_name[len-3] == 'p' && im_name[len-2] == 'n' && im_name[len-1] == 'g' )
		ret = true;
	if ( im_name[len-4] == '.' && im_name[len-3] == 'P' && im_name[len-2] == 'N' && im_name[len-1] == 'G' )
		ret = true;

	return ret;
}

int __stdcall New_Dlg_CustomReq_Proc(_CustomDlg_* dlg, _EventMsg_* msg)
{
	int r = dlg->DefProc(msg);

	// if (_DlgTextEdit_* edit != 0)
	if ( dlg->custom_data[0] != 0) {
		dlg->SetFocuseToItem(256);
		dlg->custom_data[0] = 0;
	}

	if (msg->type == MT_MOUSEOVER) // ведение мыши
	{
		_DlgStaticTextPcx8ed_* statBar = (_DlgStaticTextPcx8ed_*)dlg->GetItem(115);
		_DlgItem_* it = dlg->FindItem(msg->x_abs, msg->y_abs);
		char* text = o_NullString;
		if (it)	
		{
			if ( it->short_tip_text ) 
				text = it->short_tip_text;

			statBar->SetText(text);
			statBar->Draw();
			statBar->RedrawScreen();

		}
	} // type == MT_MOUSEOVER 

	if (msg->type == MT_MOUSEBUTTON)
	{
		if (msg->subtype == MST_LBUTTONCLICK) // ЛКМ при отжатии
		{
			if (msg->item_id >= 15 && msg->item_id <= 18) 
			{
				_DlgItem_* item;
				_DlgStaticTextPcx8ed_* itemText;
				for (int i = 0; i < 4; i++) 
				{
					item = dlg->GetItem(25+i);					
					if (item) {	item->SendCommand(6, 4); }

					itemText = (_DlgStaticTextPcx8ed_*)dlg->GetItem(19+i);
					if (itemText)
					{
						itemText->font = (int)smalfont2;
						itemText->color = 1;
					}
				}

				dlg->GetItem(msg->item_id +10)->SendCommand(5, 4);
				((_DlgStaticTextPcx8ed_*)dlg->GetItem(msg->item_id +4))->font = (int)medfont2;
				((_DlgStaticTextPcx8ed_*)dlg->GetItem(msg->item_id +4))->color = 7;
				o_Sphinx1->SelItm = msg->item_id -14;

				dlg->GetItem(DIID_OK)->SetEnabled(1);
				dlg->Redraw();
			}
		} // subtype == MST_LBUTTONCLICK

		//if (msg->subtype == MST_LBUTTONDOWN)  // ЛКМ при нажатии
		//{		//} // subtype == MST_LBUTTONDOWN
	} // type == MT_MOUSEBUTTON

	// dlg->Redraw();

	return r;
}



int New_Dlg_CustomReq(_Sphinx1_* Sphinx)
{
	// высчитываем размеры диалога
	int x = 540;
	int y = 0;
	int yy = 120;
	int lines = 0;
	
	int lines1 = 0;
	int h_text1 = 0;
	int h_text2 = 0;
	int h_text3 = 0;
	int h_pic = 0;
	
	if (Sphinx->Text1) {
		lines1 = medfont2->GetLinesCountInText(Sphinx->Text1, x -40);
		h_text1 = lines1 *16 +15;
		yy += h_text1 ;
	}
	int cansel_show = Sphinx->ShowCancel;
	if (Sphinx->Text2) {
		h_text2 = 16;
		yy += h_text2 +26; // 26 выделяем высоту на поле ввода текста
	} 

	if (Sphinx->Text3 ) {
		h_text3 = 16;
		yy += h_text3;
	}

	if (Sphinx->Text2 && Sphinx->Text3 ) {
		yy -= h_text3;
	}

	if (!Sphinx->Text2 && !Sphinx->Text3 ) {
		cansel_show = 0;
	}

	if (Sphinx->Pic1Path || Sphinx->Pic2Path) {
		h_pic = 100;
		yy += h_pic;
	}

	int count_bttns = 0;
	if (Sphinx->Text3) {
		if (Sphinx->Chk1Text) { yy += 27; count_bttns++; }
		if (Sphinx->Chk2Text) { yy += 27; count_bttns++; }
		if (Sphinx->Chk3Text) { yy += 27; count_bttns++; }
		if (Sphinx->Chk4Text) { yy += 27; count_bttns++; }
	}

	if (count_bttns > 1) {
		if (Sphinx->Text2) { 
			yy -= 32;
		}
	}

	// проверка на выход за границы экрана 600px по вертикали
	if (yy > 580) {
		h_text1 -= yy - 580;
		yy = 580;
	}

	y = yy;

	_CustomDlg_* dlg = _CustomDlg_::Create(-1, -1, x, y, DF_SCREENSHOT | DF_SHADOW, New_Dlg_CustomReq_Proc);
	Set_DlgBackground_RK(dlg, 1, o_GameMgr->GetMeID());

	_DlgStaticText_* Text1 = 0;
	if (Sphinx->Text1) {
		dlg->AddItem(Text1 = _DlgStaticText_::Create(24, 20, dlg->width -48, h_text1, Sphinx->Text1, n_medfont2, 1, 4, ALIGN_H_CENTER | ALIGN_V_CENTER, 0));
	}


	int ptr_pic1 = (int)&Sphinx->Pic1Path;
	int count_pics = 0; // считаем кол-во изображений
	for (int i = 0; i < 4; i++) {
		if ( *(char**)(ptr_pic1 +4*i) )
			count_pics++;
	}

	char* pic_name = o_NullString;
	// строим изображения id 10-14
	if (count_pics) {
		int max_width_pic = 400 / count_pics;
		for (int i = 0; i < count_pics; i++) {
			int delta = x/(count_pics+1);
			int start_x = (delta - (max_width_pic/2) ) + (delta*i);
			int start_y = 24+h_text1;
			
			char* pPath = *(char**)(ptr_pic1 +4*i);
			char* pHint = *(char**)(ptr_pic1 +4*i +16);
		
			char* short_name = GetShortFileName_Y(pPath);
			if (IsSupportedFormatImage(short_name) )
			{
				// грузим картинку через era.dll->LoadImageAsPcx16()
				_Pcx16_* o_Pic = (_Pcx16_*)Era::LoadImageAsPcx16(pPath, short_name, 0, 0, max_width_pic, 100, 3); 
				
				int pic_x = 0;
				int pic_y = 0;
				if (o_Pic) 
				{
					// проверяем размеры загруженной картинки
					// и при выходе за границы 100х100
					pic_x = o_Pic->width;
					pic_y = o_Pic->height;

					// вычисляем привязку изображений (координаты верхнего левого угла картинки)
					start_x += ( (max_width_pic/2) - pic_x/2);
					start_y += (50 - pic_y/2);

					dlg->AddItem(_DlgStaticPcx16_::Create(start_x, start_y, pic_x, pic_y, 10+i, o_Pic->name, 2048));
					// dlg->GetItem(10+i)->full_tip_text = pPath; // ПКМ на картинке
					if (pHint)
					{
						dlg->GetItem(10+i)->full_tip_text = o_NullString;
						dlg->GetItem(10+i)->short_tip_text = pHint;
					}
				}
			} 
			else 
			{
				dlg->AddItem(_DlgStaticText_::Create(start_x, 24+h_text1, 100, 100, pPath, n_smallfont2, 1, 50+i, ALIGN_H_CENTER | ALIGN_V_CENTER, 0));
				b_YellowFrame_Create(dlg, start_x, 24+h_text1, 100, 100, 10+i, ON, o_Pal_Y);
				dlg->GetItem(10+i)->full_tip_text = pPath; // ПКМ на картинке
				if (pHint){
					dlg->GetItem(10+i)->short_tip_text = pHint;
				}

				// в диалоге неподдерживаемая картинка gif/avi, поэтому выходим из нашего диалога
				// и передадим управление воговскому из zvslib.dll который их может обработать
				i = 4; // счетчик цикла в максимум
				Sphinx->SelItm = 10;
				dlg->Destroy(TRUE);
				return Sphinx->SelItm; // 10 = значит нужно грузить стандартный диалог
			} // end else IsSupportedFormatImage()
		} // end for()
	} // end if (count_pics)

	if (Sphinx->Text2) {
		dlg->AddItem(_DlgStaticText_::Create(24, 30+h_text1+h_pic, dlg->width -48, h_text2, Sphinx->Text2, n_medfont2, 7, 5, ALIGN_H_CENTER | ALIGN_V_CENTER, 0));
	}

	if (Sphinx->Text3) {
		dlg->AddItem(_DlgStaticText_::Create(24, 30+h_text1+h_pic, dlg->width -48, h_text3, Sphinx->Text3, n_medfont2, 7, 6, ALIGN_H_CENTER | ALIGN_V_CENTER, 0));
	}

	if (Sphinx->Text2 && Sphinx->Text3)
	{
		dlg->GetItem(5)->x = 36;
		dlg->GetItem(5)->width = x/2 -48;

		// смещаем вправо Text3
		dlg->GetItem(6)->x = x/2 +12;
		dlg->GetItem(6)->width = x/2 -48;
	}

	_DlgTextEdit_* edit_text = 0;
	dlg->custom_data[0] = 0; // всегда (но для единичного раза установка будет 1)
	if (Sphinx->Text2) {
		// создаём кнопки id 15-18
		dlg->custom_data[0] = 1;  // один раз установить фокус на ввод текста
		int editText_y = (dlg->GetItem(5)->y) + h_text2 +11;
		int editText_x = dlg->GetItem(5)->x;
		int editText_width = dlg->GetItem(5)->width;

		// поле ввода текста
		if (editText_width > 250) {
			editText_width = 220;
			editText_x = x/2 -110;
		}
		b_YellowFrame_Create(dlg, editText_x, editText_y -1, editText_width +1, 20, 8, ON, o_Pal_Grey);
		// b_YellowFrame_Create(dlg, editText_x -2, editText_y -2, editText_width +4, 22, 8, ON, o_Pal_Y);

		dlg->AddItem(edit_text = _DlgTextEdit_::Create(editText_x+2, editText_y, editText_width-2, 18, 64, o_NullString, n_medfont2, 1, ALIGN_H_LEFT | ALIGN_V_CENTER, "WoGTextEdit.pcx", 256, 4, 0, 0));
		dlg->GetItem(256)->full_tip_text = o_NullString;
		dlg->GetItem(256)->short_tip_text = Sphinx->Text2;
	}

	if (Sphinx->Text3) {
		// создаём кнопки id 15-18 (19-22)
		int bttn_y = (dlg->GetItem(6)->y) + h_text3 +10;
		int bttn_x = dlg->GetItem(6)->x;
		int bttn_width = dlg->GetItem(6)->width;

		if (Sphinx->Chk1Text) {
			dlg->AddItem(_DlgStaticTextPcx8ed_::Create(bttn_x+1, bttn_y+1, bttn_width-2, 20, Sphinx->Chk1Text, n_smallfont2, "AdRollvr.pcx", 1, 19, ALIGN_H_CENTER | ALIGN_V_CENTER) );
			b_YellowFrame_Create(dlg, bttn_x-1, bttn_y-1, bttn_width+2, 23, 25, OFF, o_Pal_Y);
			b_YellowFrame_Create(dlg, bttn_x, bttn_y, bttn_width, 21, 15, ON, o_Pal_Grey);
			dlg->GetItem(15)->full_tip_text = o_NullString;
			dlg->GetItem(15)->short_tip_text = Sphinx->Chk1Hint;
			bttn_y += 25;			
		}

		if (Sphinx->Chk2Text) {
			dlg->AddItem(_DlgStaticTextPcx8ed_::Create(bttn_x+1, bttn_y+1, bttn_width-2, 20, Sphinx->Chk2Text, n_smallfont2, "AdRollvr.pcx", 1, 20, ALIGN_H_CENTER | ALIGN_V_CENTER) );
			b_YellowFrame_Create(dlg, bttn_x-1, bttn_y-1, bttn_width+2, 23, 26, OFF, o_Pal_Y);
			b_YellowFrame_Create(dlg, bttn_x, bttn_y, bttn_width, 21, 16, ON, o_Pal_Grey);
			dlg->GetItem(16)->full_tip_text = o_NullString;
			dlg->GetItem(16)->short_tip_text = Sphinx->Chk2Hint;
			bttn_y += 25;			
		}

		if (Sphinx->Chk3Text) {
			dlg->AddItem(_DlgStaticTextPcx8ed_::Create(bttn_x+1, bttn_y+1, bttn_width-2, 20, Sphinx->Chk3Text, n_smallfont2, "AdRollvr.pcx", 1, 21, ALIGN_H_CENTER | ALIGN_V_CENTER) );
			b_YellowFrame_Create(dlg, bttn_x-1, bttn_y-1, bttn_width+2, 23, 27, OFF, o_Pal_Y);
			b_YellowFrame_Create(dlg, bttn_x, bttn_y, bttn_width, 21, 17, ON, o_Pal_Grey);
			dlg->GetItem(17)->full_tip_text = o_NullString;
			dlg->GetItem(17)->short_tip_text = Sphinx->Chk3Hint;
			bttn_y += 25;			
		}

		if (Sphinx->Chk4Text) {
			dlg->AddItem(_DlgStaticTextPcx8ed_::Create(bttn_x+1, bttn_y+1, bttn_width-2, 20, Sphinx->Chk4Text, n_smallfont2, "AdRollvr.pcx", 1, 22, ALIGN_H_CENTER | ALIGN_V_CENTER) );
			b_YellowFrame_Create(dlg, bttn_x-1, bttn_y-1, bttn_width+2, 23, 28, OFF, o_Pal_Y);
			b_YellowFrame_Create(dlg, bttn_x, bttn_y, bttn_width, 21, 18, ON, o_Pal_Grey);
			dlg->GetItem(18)->full_tip_text = o_NullString;
			dlg->GetItem(18)->short_tip_text = Sphinx->Chk4Hint;
			bttn_y += 25;
		}
	}

	int x_center = x / 2; // вычисляем середину ширины диалога

	if ( cansel_show )
	{
		dlg->AddItem(_DlgStaticPcx8_::Create(x_center +16, dlg->height -74, 0, "Box64x30.pcx"));
		dlg->AddItem(_DlgButton_::Create(x_center +17, dlg->height -73, 64, 30, DIID_CANCEL, "iCancel.def", 0, 1, 1, 1, 2)); 
		dlg->GetItem(DIID_CANCEL)->full_tip_text = o_NullString;
		dlg->GetItem(DIID_CANCEL)->short_tip_text = txtresWOG->GetString(12);
		x_center -= 47; // для правильного смещения кнопки ОК
	} 
	dlg->AddItem(_DlgStaticPcx8_::Create(x_center -33, dlg->height -74, 0, "Box64x30.pcx"));
	dlg->AddItem(_DlgButton_::Create(x_center -32, dlg->height -73, 64, 30, DIID_OK, "iOkay.def", 0, 1, 1, 28, 2));
	dlg->GetItem(DIID_OK)->full_tip_text = o_NullString;
	dlg->GetItem(DIID_OK)->short_tip_text = txtresWOG->GetString(11);

	if ( count_bttns > 0) {	
		dlg->GetItem(DIID_OK)->SetEnabled(0); 
	}

	// (id = 115) подсказка в статус баре	
	dlg->AddItem(_DlgStaticTextPcx8ed_::Create(8, dlg->height -18 -8, dlg->width - 16, 18, o_NullString, n_smallfont2, "AdRollvr.pcx", 1, 115, ALIGN_H_CENTER | ALIGN_V_CENTER) ); // HD_TStat.pcx


	Sphinx->SelItm = 0;
	dlg->Run();

	// если был создан диалог с вводом текста: возвращаем введенный текст
	if ( edit_text ) { 		
		Sphinx->Text4 = edit_text->text;
	}

	dlg->Destroy(TRUE);

	if (o_WndMgr->result_dlg_item_id == DIID_CANCEL){ 
		Sphinx->SelItm = -1; 
	}
	if (o_WndMgr->result_dlg_item_id == DIID_OK) { 
		if (!Sphinx->SelItm) {
			Sphinx->SelItm = 5;	
		}
	}

	// если возвращается 10, то нужно вызвать стандартный воговский диалог (есть avi или gif)
	return Sphinx->SelItm;
}


int __stdcall Y_Dlg_CustomReq(LoHook* h, HookContext* c)
{	
	Dlg_CustomReq_Ban++;
	if (Dlg_CustomReq_Ban == 1) 
	{
		o_PauseVideo();

		_Sphinx1_* Sphinx = (_Sphinx1_*)(c->ebp -0x260);
		o_Sphinx1 = Sphinx; // делаем глобальное сохранение адреса класса, чтобы потом при необходимости иметь прямой доступ

		int result = 0;

		if (Sphinx->Text2 && BanDlg_CustomReq_EnterText /* GetWoGOptionsStatus(OPTION_ID) */ ) {			
			result = 10; // гребанные выкрутасы. Мне такие костыли очень, очень не нравятся!
		}

		if (result != 10) 
		{
			int save_WOG_DisableMouse = WOG_DisableMouse;
			WOG_DisableMouse = 0; // отключаем блок изменения кадров курсора
			// запоминаем кадр курсора мыши
			int cursor_t = o_MouseMgr->Field<_int_>(+0x4C);
			int cursor_f = o_MouseMgr->Field<_int_>(+0x50);
			b_MouseMgr_SetCursor(0,0);
			CALL_2(void*, __thiscall, 0x50D7B0, o_MouseMgr, 0);

			result = New_Dlg_CustomReq(Sphinx);

			b_MouseMgr_SetCursor(cursor_f, cursor_t);
			WOG_DisableMouse = save_WOG_DisableMouse;
		}

		if (result == 10) 
		{ // в диалоге gif/avi = грузим стандартный воговский диалог
			CALL_0(void, __stdcall, 0x771978); //  WoG_BeforeDialog()  
			int soundEffect = CALL_0(int, __cdecl, 0x71666C) > 4;
			int v5 = CALL_0(int, __cdecl, 0x771116); // int __cdecl Service_GetForegroundWindow()
			result = CALL_4(int, __cdecl, *(int*)0x2878F1C, v5, (int)Sphinx, /* ".\\Data\\ZVS\\LIB1.RES"*/ (char*)0x7A4DC8, soundEffect);
			int v6 = CALL_0(int, __cdecl, 0x771116); // int __cdecl Service_GetForegroundWindow()
			// CALL_1(int, __cdecl, *(int*)0x7850F0, v6); // WOG_SetFocus()
			CALL_0(void, __stdcall, 0x77186A);  // WOG_AfterDialog()
		}

		o_ContinueVideo();
		Dlg_CustomReq_Ban = 0;
	} 

	c->return_address = 0x772CF2;
	return NO_EXEC_DEFAULT;
} 

/*
// 0x772D50 диалог посещения камней силы (повышение перв.навыков командира)
int __stdcall Y_Dlg_CustomReq2(HiHook* hook, _Sphinx1_* Sphinx)
{
	Sphinx->Pic1Path = Sphinx->Pic2Path;

	New_Dlg_CustomReq(Sphinx); // диалог

	//sprintf(o_TextBuffer, "res: %d \n\n %s \n\n %s \n %s", 
	//	Sphinx->SelItm,
	//	Sphinx->Text1,
	//	Sphinx->Pic1Path,
	//	Sphinx->Pic2Path);
	//b_MsgBox(o_TextBuffer, 1);

	// return CALL_1(int, __cdecl, hook->GetDefaultFunc(), Sphinx);
	return 0;
}
*/ 

// диалог вопросов Сфинкса
int __stdcall Y_WoGDlg_SphinxReq(HiHook* hook, int Num) 
{
	// if ( если окно ввода должно быть отключено (и необходима работа стандартного воговского (опция 911)) )
	if ( BanDlg_CustomReq_EnterText /* GetWoGOptionsStatus(OPTION_ID) */  )
	{
		// вызов оригинальной функции
		return CALL_1(int, __cdecl, hook->GetDefaultFunc(), Num);
	} 
	else 
	{
		_Sphinx1_ Sphinx;
		Sphinx.SelItm = -1;
		Sphinx.Text1 = CALL_3(char*, __cdecl, 0x77710B, Num, 0, 0x289BFF0); 
		Sphinx.Text2 = CALL_3(char*, __cdecl, 0x77710B, 122, 0, 0x7C8E3C); 
		Sphinx.Text3 = 0;
		*(_byte_*)0x28AAB88 = 57; // Answer[0]='9';
		*(_byte_*)0x28AAB89 = 57; // Answer[1]='9';
		*(_byte_*)0x28AAB8A = 57; // Answer[2]='9';
		*(_byte_*)0x28AAB8B = 57; // Answer[3]='9';
		*(_byte_*)0x28AAB8C = 0;  // Answer[4]=0;

		Sphinx.Text4 = (char*)0x28AAB88;
		Sphinx.Pic1Path = 0;
		Sphinx.Pic2Path = 0;
		Sphinx.Pic3Path = 0;
		Sphinx.Pic4Path = 0;
		Sphinx.Pic1Hint = 0;
		Sphinx.Pic2Hint = 0;
		Sphinx.Pic3Hint = 0;
		Sphinx.Pic4Hint = 0;
		Sphinx.Chk1Text = 0;
		Sphinx.Chk2Text = 0;
		Sphinx.Chk3Text = 0;
		Sphinx.Chk4Text = 0;
		Sphinx.Chk1Hint = 0;
		Sphinx.Chk2Hint = 0;
		Sphinx.Chk3Hint = 0;
		Sphinx.Chk4Hint = 0;
		Sphinx.ShowCancel = 0;

		// делаем глоб.ссылку на Sphinx
		o_Sphinx1 = (_Sphinx1_*)&Sphinx;

		// запоминаем кадр курсора мыши
		int save_WOG_DisableMouse = WOG_DisableMouse;
		WOG_DisableMouse = 0; // разблокировали
		int cursor_t = o_MouseMgr->Field<_int_>(+0x4C);
		int cursor_f = o_MouseMgr->Field<_int_>(+0x50);
		b_MouseMgr_SetCursor(0,0);
		CALL_2(void*, __thiscall, 0x50D7B0, o_MouseMgr, 0);

		New_Dlg_CustomReq(o_Sphinx1); // диалог

		// возвращаем курсор
		b_MouseMgr_SetCursor(cursor_f, cursor_t);
		WOG_DisableMouse = save_WOG_DisableMouse;

		CALL_3(void, __cdecl, 0x710B9B, 0x28AAB88, 512, Sphinx.Text4); // WoG_StrCopy(Answer, int 512, Sphinx.Text4)
		return CALL_2(int, __cdecl, 0x772DFD, 0x28AAB88, CALL_3(char*, __cdecl, 0x77710B, Num, 1, 0x289BFF0));
	}
}
 

void Dlg_Sphinx(PatcherInstance* _PI)
{

	// Диалог IF:D/E
	_PI->WriteCodePatch(0x772A6C, "%n", 5); // call    WoG_BeforeDialog()
	_PI->WriteCodePatch(0x772D39, "%n", 5); // call    WOG_AfterDialog()		
	_PI->WriteLoHook(0x772CBD, Y_Dlg_CustomReq);
	// проверяем параметр (будем ли показывать диалог ввода текста в диалоге IF:D/E)
	// Dlg_CustomReq_IsEnterText_Ban();

	// диалог посещения камней силы командиров
	// _PI->WriteHiHook(0x772D50, SPLICE_, EXTENDED_, CDECL_, Y_Dlg_CustomReq2);

	// диалог сфинкса
	_PI->WriteHiHook(0x772E48, SPLICE_, EXTENDED_, CDECL_, Y_WoGDlg_SphinxReq);
}



