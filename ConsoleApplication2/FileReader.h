#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
#pragma once

namespace ConsoleApplication2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FileReader
	/// </summary>
	public ref class FileReader : public System::Windows::Forms::Form
	{
	public:
		void MensagemDispathError(_In_opt_ int ErrorCode) {
			void* Alloc__ = malloc(1024);
			WCHAR* ErrorMensagem = (WCHAR*)Alloc__;
			if (ErrorCode == 0)
				FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), NULL, ErrorMensagem, 260, NULL);
			else
				FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ErrorCode, NULL, ErrorMensagem, 260, NULL);
			MessageBoxW((HWND)this->Handle.ToInt32(), ErrorMensagem, NULL, MB_ICONERROR);
			this->Close();
			free(Alloc__);
		}

		FileReader(string FR_FileLocal, Form^ ParentForm)
		{
			InitializeComponent();

			int locX, locY;
			locX = ParentForm->Location.X + ParentForm->Size.Width / 2;		/*PontoX da metade da janela.*/
			locY = ParentForm->Location.Y + ParentForm->Size.Height / 2;	/*PontoY da metade da jaela.*/

			locX = locX - (this->Size.Width / 2);		/*Centralizar X do form na janela*/
			locY = locY - (this->Size.Height / 2);		/*Centralizar Y do form na janela*/

			/*Pegar as posições para colocar o form calc ao lado do form principal*/
			//this->SetDesktopLocation(locX, locY);
			this->Show(ParentForm);

			MoveWindow((HWND)this->Handle.ToInt32(), locX, locY, this->Size.Width, this->Size.Height, true);
			/*HWND = Janela(identificador da janela)*/
			/*locX e locY, são localizações da janela, respectivamente, x e y.*/
			/*this->Size. (Width e Height) são respectivamente, largura e altura.*/
			/*Último parâmetro é um BOOL (true ou false), que na função server pintar a janela. Ao trocar a posição de janela
			sobra alguns pixeis em branco na janela, logo precisa repintar a janela. 
			As funções UpdateWindow e RedrawWindow, fazem exatamente isso.*/

			HANDLE FR_FileHandle = CreateFileA(FR_FileLocal.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
			
			if (FR_FileHandle == INVALID_HANDLE_VALUE)
			{
				FR_FileLocal.clear();
				MensagemDispathError(NULL);
				return;
			}

			GetFileType(FR_FileHandle);

			string FR_FileContent;
			CHAR *FR_FileBuffer;
			DWORD FR_FileSizeHigh = 0;											/*Parte 'high'(ALTA) do arquivo | Resto do arquivo*/
			DWORD FR_FileSize	= GetFileSize(FR_FileHandle, &FR_FileSizeHigh);	/*Pega o tamanho do arquivo em bytes*/
			DWORD FR_FileBytes	= 0;

			HLOCAL FR_LocalBuffer = LocalAlloc(LMEM_FIXED, FR_FileSize + 1);	/*O tamanho do buffer deve ser o tamanho do arquivo ou maior*/
			LocalLock(FR_LocalBuffer);

			if (LocalSize(FR_LocalBuffer) < FR_FileSize)
			{
				FR_FileLocal.clear();
				LocalUnlock(FR_LocalBuffer);
				LocalFree(FR_LocalBuffer);
				CloseHandle(FR_FileHandle);
				MensagemDispathError(NULL);
				return;
			}
			else{
				FR_FileBuffer = (CHAR*)FR_LocalBuffer;

				while (true)
				{
					if (!ReadFile(FR_FileHandle, FR_FileBuffer, FR_FileSize, &FR_FileBytes, NULL))
					{
						FR_FileLocal.clear();
						ZeroMemory(FR_FileBuffer, sizeof(FR_FileBuffer));
						LocalUnlock(FR_LocalBuffer);
						LocalFree(FR_LocalBuffer);
						CloseHandle(FR_FileHandle);
						MensagemDispathError(NULL);
						return;
					}
					if (FR_FileBytes == NULL)					
						break;

					else
						FR_FileContent += FR_FileBuffer;
				}

				if (FR_FileSize > 3 && textBox1->Text->Length <= 3)
				{
					HANDLE	FR_FileMap = CreateFileMappingW(FR_FileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
					PVOID	FR_MapView = MapViewOfFile(FR_FileMap, FILE_MAP_READ, 0, 0, 0);
					FlushFileBuffers(FR_FileHandle);
					FR_FileContent.clear();
					textBox1->Text = String::Empty;

					for (int i = 0; i < FR_FileSize; i++)
					{
						if (*((char*)FR_MapView + i) != '\0')
							FR_FileContent += *((char*)FR_MapView + i);
					}
					textBox1->Text = gcnew String(FR_FileContent.c_str());
					UnmapViewOfFile(FR_MapView);
					CloseHandle(FR_FileMap);
				}
				CloseHandle(FR_FileHandle);
				textBox1->Text = gcnew String(FR_FileContent.c_str());
				textBox1->SelectionStart = 0;
				textBox1->SelectionLength = 0;
				
				/*Limpeza na memória*/
				FR_FileLocal.clear();
				ZeroMemory(FR_FileBuffer, sizeof(FR_FileBuffer));
				LocalUnlock(FR_LocalBuffer);
				LocalFree(FR_LocalBuffer);
				CloseHandle(FR_FileHandle);
				FR_FileContent.clear();
			}
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FileReader()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^  textBox1;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->BackColor = System::Drawing::SystemColors::ActiveBorder;
			this->textBox1->Location = System::Drawing::Point(12, 12);
			this->textBox1->MaxLength = 0;
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox1->Size = System::Drawing::Size(587, 342);
			this->textBox1->TabIndex = 0;
			// 
			// FileReader
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(611, 366);
			this->Controls->Add(this->textBox1);
			this->Name = L"FileReader";
			this->Text = L"FileReader byDennys";
			this->SizeChanged += gcnew System::EventHandler(this, &FileReader::FileReader_SizeChanged);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void FileReader_SizeChanged(System::Object^  sender, System::EventArgs^  e) {
		textBox1->Width = this->Width - 40; //X
		textBox1->Height = this->Height - 62;
	}
	};
}
