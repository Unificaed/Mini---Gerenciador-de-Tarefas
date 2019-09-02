#include "Handles.h"
#include "Calc1.h"
#include "FileReader.h"

#pragma comment(lib, "WtsApi32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "ntdll.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Pdh.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Shlwapi.lib")

#pragma warning(push)
#pragma warning(disable:4201)
#pragma warning(disable:4820)
#pragma warning(disable:4324)
#pragma warning(disable:4471)

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Threading;
using namespace System::Globalization;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

HRESULT GetUserFromProcess(const DWORD procId, std::wstring& strUser, std::wstring& strdomain)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, procId);
	if (hProcess == NULL)
		return E_FAIL;

	HANDLE hToken = NULL;

	if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
	{
		CloseHandle(hProcess);
		return E_FAIL;
	}
	DWORD dwSize = 256;
	BOOL bSuccess = FALSE;
	DWORD dwLength = 0;
	strUser.clear();
	strdomain.clear();

	PTOKEN_USER ptu = NULL;

	if (NULL == hToken)
		goto Cleanup;

	if (!GetTokenInformation(hToken, TokenUser, (LPVOID)ptu, 0, &dwLength))
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			goto Cleanup;

		ptu = (PTOKEN_USER)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwLength); /*Alocação de memória*/

		if (ptu == NULL)
			goto Cleanup;
	}

	if (!GetTokenInformation(hToken, TokenUser, (LPVOID)ptu, dwLength, &dwLength))
		goto Cleanup;

	SID_NAME_USE SidType;
	wchar_t lpName	[256];
	wchar_t lpDomain[256];

	if (!LookupAccountSidW(NULL, ptu->User.Sid, lpName, &dwSize, lpDomain, &dwSize, &SidType))
		goto Cleanup;
	else
	{
		strUser = lpName;
		strdomain = lpDomain;
		bSuccess = TRUE;
	}

Cleanup:

	if (ptu != NULL)
		HeapFree(GetProcessHeap(), 0, (LPVOID)ptu);

	CloseHandle(hToken);
	CloseHandle(hProcess);
	return bSuccess;
}

namespace ConsoleApplication2 {
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		::std::string* Modulos = new std::string[2048]();
		int LargInicial, LargMax;
		float SleepTime;
	private: 
			System::Windows::Forms::Button^  button2, ^button3, ^button4, ^button5, ^button6, ^button7, ^button8, ^button9, ^button10;
			System::Windows::Forms::Button^TT_B5, ^TT_B4, ^TT_B3, ^TT_B2, ^  TT_B1, ^TT_B0;
			System::Windows::Forms::ListBox^  listBox2;
			System::Windows::Forms::DataGridView^ dataGridView1, ^dataGridView2;
			System::Windows::Forms::MenuStrip^  menuStrip2;
			 
			System::Windows::Forms::ToolStripMenuItem^  viewToolStripMenuItem, ^atualizaçãoAutomáticaToolStripMenuItem,
			^atualizarToolStripMenuItem, ^tempoDeAtualizaçãoToolStripMenuItem, ^segundosToolStripMenuItem, ^segundoToolStripMenuItem,
			^segundosToolStripMenuItem1, ^segundosToolStripMenuItem2, ^opacidadeToolStripMenuItem, ^toolStripMenuItem2,
			^toolStripMenuItem3, ^toolStripMenuItem4, ^toolStripMenuItem5, ^toolStripMenuItem6, ^toolStripMenuItem7,
			^toolStripMenuItem8, ^toolStripMenuItem9, ^toolStripMenuItem10, ^visívelToolStripMenuItem, ^sempreVisívelToolStripMenuItem,
			^ferramentasToolStripMenuItem, ^calculadoraToolStripMenuItem, ^leitorDeTextoToolStripMenuItem;
			 
			System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1, ^toolStripSeparator2;
			System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
			System::Windows::Forms::ComboBox^  comboBox1;	
			System::Windows::Forms::StatusStrip^  statusStrip1;
			System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1, ^toolStripStatusLabel2;
			 
			System::Windows::Forms::TabControl^  tabControl1;
			System::Windows::Forms::TabPage^  tabPage1, ^tabPage2;
			System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn1, ^dataGridViewTextBoxColumn2,
			 ^dataGridViewTextBoxColumn3, ^dataGridViewTextBoxColumn4, ^Column6;
	private: System::Windows::Forms::DataGridViewImageColumn^  Column4;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column5;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
	private: System::Windows::Forms::Label^  label1;

	public:
		HWND janelaAtual;  /*HANDLE do form na qual a mensagem será fixada*/

		void MensagemDispathError(_In_opt_ int ErrorCode) { /*_In_opt_ = OPCIONAL, facilita na digitação :D*/

			void* Alloc__ = malloc(1024);
			WCHAR* ErrorMensagem = (WCHAR*)Alloc__;
			if (ErrorCode == 0)
				FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), NULL, ErrorMensagem, 260, NULL);
			else
				FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ErrorCode, NULL, ErrorMensagem, 260, NULL);
			/*FormatMessage -> Traduz o error code para o idioma local; Cada erro possui seu número(error code), que pode ser traduzido*/
			MessageBoxW(janelaAtual, ErrorMensagem, NULL, MB_ICONERROR);
			free(Alloc__);
		}

		HANDLE TemporizadorRefresh = INVALID_HANDLE_VALUE;
		LARGE_INTEGER TemporizadorInteger(float Tempo) {
			LARGE_INTEGER NewInteger__;
			NewInteger__.QuadPart = Tempo * 10000000; /*Tempo a ser definido*/
			return NewInteger__;
		}


		void AjustPrivilegios()
			/*Ajustar os privilégios do token do usuário = Acesso a todos os processos protegidos pelo sistema */
		{
			HANDLE hToken = NULL;
			TOKEN_PRIVILEGES tokenPriv;
			LUID luidDebug;
			OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
			LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luidDebug);
			tokenPriv.PrivilegeCount = 1;
			tokenPriv.Privileges[0].Luid		= luidDebug;
			tokenPriv.Privileges[0].Attributes	= SE_PRIVILEGE_ENABLED;
			AdjustTokenPrivileges(hToken, FALSE, &tokenPriv, 0, NULL, NULL);
			CloseHandle(hToken);
		}

		List<DataGridViewRow^>^ GridColorChange = gcnew List<DataGridViewRow^>();
		List<DataGridViewRow^>^ GridColorChange2 = gcnew List<DataGridViewRow^>();
		BOOL needrefresh = FALSE;
		Bitmap ^ PadraoProcessIcon = nullptr;
		Bitmap ^ YahooZ;

		std::vector<string*>*	HELP_Modulos	= new std::vector<string*>;


		void AttModuleByVector(std::vector<string> Yahv)
		{

		}

		void ProcessosAtt(bool Inicio)
		{
			System::Drawing::Icon^ icon;

			PROCESSENTRY32W procEntry;
			procEntry.dwSize = sizeof(PROCESSENTRY32W);
			HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

			int ProcessosContagem = 0l;	//Quantidade de processos

				List<DWORD>^ processosatuais = gcnew List<DWORD>();

				SendMessageW((HWND)dataGridView1->Handle.ToInt32(), WM_SETREDRAW, FALSE, 0);
				needrefresh = FALSE;
				for (int i = 0, c1 = 0, c2 = 0;; i++)
				{
					if (i < GridColorChange->Count)
					{
						if (GridColorChange[i]->DefaultCellStyle->BackColor != Color::Empty)
						{
							GridColorChange[i]->DefaultCellStyle->BackColor = Color::Empty;
							needrefresh = TRUE;
						}
						c1 = 1;
					}
					if (i < GridColorChange2->Count)
					{
						if (GridColorChange2[i]->Visible == true)
						{
							dataGridView1->Rows->Remove(GridColorChange2[i]);
							needrefresh = TRUE;
						}

						c2 = 1;
					}
					if (c1 == 1 || c2 == 1)
						c1 = c2 = 0;
					else
					{
						GridColorChange->Clear();
						GridColorChange2->Clear();
						break;
					}
				}
				SendMessageW((HWND)dataGridView1->Handle.ToInt32(), WM_SETREDRAW, TRUE, 0);
				
				if (needrefresh == TRUE)
				{
					dataGridView1->Refresh();
					needrefresh = FALSE;
				}

				int rowscont = dataGridView1->Rows->Count;

				for (int i = 0; i < rowscont; i++)
					processosatuais->Add(Convert::ToInt32(dataGridView1->Rows[i]->Cells["Column2"]->Value->ToString()));

				List<DWORD>^ processes_ = gcnew List<DWORD>();

				SendMessageW((HWND)dataGridView1->Handle.ToInt32(), WM_SETREDRAW, FALSE, 0);
				
				Process32First(hSnapshot, &procEntry);
				do
				{
					HELP_Modulos[ProcessosContagem][0][0] = std::to_string(procEntry.th32ProcessID);
					ProcessosContagem++;
					processes_->Add(procEntry.th32ProcessID);

					if (!(processosatuais->IndexOf(procEntry.th32ProcessID) == -1))
						continue;
					else
					{
						if (!needrefresh)
							needrefresh = TRUE;
						HANDLE hOpenProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION, 0, procEntry.th32ProcessID);

						BOOL Process3264 = FALSE;
						IsWow64Process(hOpenProc, &Process3264);

						std::wstring Logon, User;
						HRESULT PROCESS_UserGetted = GetUserFromProcess(procEntry.th32ProcessID, User, Logon);

						System::String^ processoUsuario;

						YahooZ = PadraoProcessIcon;

						if (PROCESS_UserGetted != E_FAIL)
						{
							processoUsuario = gcnew System::String(std::wstring((std::wstring)Logon.c_str() + L"\\" + User.c_str()).c_str());
							wchar_t ModuleFile__[1024];
							DWORD rec = 1024;
							QueryFullProcessImageNameW(hOpenProc, 0, ModuleFile__, &rec);

							if (GetFileAttributesW(ModuleFile__) != INVALID_FILE_ATTRIBUTES)
							{
								icon = System::Drawing::Icon::ExtractAssociatedIcon(gcnew String(ModuleFile__));
								YahooZ = gcnew Bitmap(icon->ToBitmap(), 15, 15);
							}

							SecureZeroMemory(ModuleFile__, sizeof(ModuleFile__));
						}
						else
							processoUsuario = gcnew System::String("-");
						
						
						System::String^ processoNomeStr = gcnew System::String(procEntry.szExeFile);

						if (Process3264 == TRUE)
							processoNomeStr += L" *32";						

						DWORD session = 0;
						ProcessIdToSessionId(procEntry.th32ProcessID, &session);
						GridColorChange->Add(dataGridView1->Rows->SharedRow(dataGridView1->Rows->Add(YahooZ, processoNomeStr, processoUsuario, session, procEntry.th32ProcessID)));
						
						if(!Inicio)
							GridColorChange[GridColorChange->Count-1]->DefaultCellStyle->BackColor = Color::GreenYellow;

						dataGridView1->Sort(dataGridView1->Columns["Column1"], ListSortDirection::Ascending);

						CloseHandle(hOpenProc);

						processoUsuario = String::Empty;
						processoNomeStr = String::Empty;
					}

				} while (Process32Next(hSnapshot, &procEntry));
				CloseHandle(hSnapshot);

				SendMessageW((HWND)dataGridView1->Handle.ToInt32(), WM_SETREDRAW, TRUE, 0);
				if (needrefresh == TRUE)
				{
					dataGridView1->Refresh();
					needrefresh = FALSE;
				}

				SendMessageW((HWND)dataGridView1->Handle.ToInt32(), WM_SETREDRAW, FALSE, 0);
				
					for (int i = 0; i < dataGridView1->Rows->Count; i++)
					{
						if (processes_->IndexOf(Convert::ToInt32(dataGridView1->Rows[i]->Cells["Column2"]->Value->ToString())) == -1)
						{
							if (!needrefresh)
								needrefresh = TRUE;
							GridColorChange2->Add(dataGridView1->Rows->SharedRow(i));
							GridColorChange2[GridColorChange2->Count - 1]->DefaultCellStyle->BackColor = Color::OrangeRed;
						}
					}

				processosatuais->Clear();
				processes_->Clear();

				SendMessageW((HWND)dataGridView1->Handle.ToInt32(), WM_SETREDRAW, TRUE, 0);

				if (needrefresh == TRUE)
				{
					dataGridView1->Refresh();
					needrefresh = FALSE;
				}

				toolStripStatusLabel1->Text = "Processos: " + ProcessosContagem.ToString();
				SecureZeroMemory(&procEntry, sizeof(procEntry.dwSize));	//Preenche a memória com 0 (Otimização)
				dataGridView1->ResetBindings();
		}

		DWORD lastPid = -1;
		HMODULE *hModulosPRE = new HMODULE[1024];
		bool ToLeftOrRight = false;

		public: System::Void ModulosAtt(System::Object^ __ProcessId)
		{
			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

			DWORD pid = Convert::ToInt32(__ProcessId->ToString());
			ULONG states = 0;
			if (pid == lastPid)
				return;

			button7->Enabled = false;
			button8->Enabled = false;

			SecureZeroMemory(hModulosPRE, sizeof(hModulosPRE) / sizeof(HMODULE));

			int ModCountance = 0;

			lastPid = pid;
			listBox2->Items->Clear();
			HANDLE hTemp__ = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
			if (!hTemp__ || hTemp__ == INVALID_HANDLE_VALUE || GetLastError() == ERROR_ACCESS_DENIED)
			{
				CloseHandle(hTemp__);
				goto ACESSONEGADO;
			}
			MODULEENTRY32W modEntry;
			modEntry.dwSize = sizeof(MODULEENTRY32W);

			HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);

			if (!hSnapshot || hSnapshot == INVALID_HANDLE_VALUE || GetLastError() == ERROR_ACCESS_DENIED)
			{
				CloseHandle(hTemp__);
				CloseHandle(hSnapshot);
				goto ACESSONEGADO;
			}

			Module32First(hSnapshot, &modEntry);

			wchar_t MainModule[MAX_PATH];							/*Módulo principal ou executável.*/
			GetModuleBaseNameW(hTemp__, NULL, MainModule, 260);
			System::String^ eae = gcnew System::String(MainModule);
			do{
				if (modEntry.th32ProcessID == pid)
				{
					hModulosPRE[ModCountance] = modEntry.hModule;
					ModCountance++;

					System::String^ NovoItem = gcnew System::String(modEntry.szModule);
					if (eae != NovoItem)
						listBox2->Items->Add(NovoItem);
				}

			} while (Module32Next(hSnapshot, &modEntry));
			CloseHandle(hSnapshot);

			if (listBox2->Items->Count > 0)
			{
				listBox2->Sorted = true;
				listBox2->Sorted = false;

				//listBox2->Items->RemoveAt(listBox2->Items->IndexOf(eae));
				listBox2->Items->Insert(0, eae);

				CloseHandle(hTemp__);
				return;
			}
			else
				goto ACESSONEGADO;

		ACESSONEGADO:
			listBox2->Items->Add("Acesso negado.");
		}

		HANDLE hGlobalEvent;
		DWORD pidToModule;
		Thread^ threadAttMod;

		void ThreadAlertIn(){
			while (true)
			{
				WaitForSingleObject(hGlobalEvent, INFINITE);
				threadAttMod = gcnew Thread(gcnew ParameterizedThreadStart(this, &MyForm::ModulosAtt));
				threadAttMod->Start(pidToModule);
				threadAttMod->Join();
			}
		}

		List<ToolStripMenuItem^>^ MenusTimes = gcnew List<ToolStripMenuItem^>();
		List<ToolStripMenuItem^>^ MenusVisib = gcnew List<ToolStripMenuItem^>();
		
		Thread^ backSlash;
		bool CheckedAtt = true;

		void TimeRefresh()
		{
			SYSTEMTIME LocalTimeSystem; 
			GetLocalTime(&LocalTimeSystem);
			
			char* HorarioAtualRelease = (char*)malloc(MAX_PATH); /*Char precisa de um espaço necessário na memória.*/
			//sprintf(HorarioAtualRelease, "Horário: %hu:%hu:%hu", LocalTimeSystem.wHour, LocalTimeSystem.wMinute, LocalTimeSystem.wSecond);			
			GetTimeFormatA(LOCALE_USER_DEFAULT, NULL, &LocalTimeSystem, "HH:mm:ss", HorarioAtualRelease, MAX_PATH);
			toolStripStatusLabel2->Text = gcnew String(string((string)"Horário: " + HorarioAtualRelease).c_str());
		}

		System::Void AtualizacaoProcessos()
		{
			while (true)
			{
				SetWaitableTimer(TemporizadorRefresh, &TemporizadorInteger(SleepTime), 0, NULL, NULL, 0);
				WaitForSingleObjectEx(TemporizadorRefresh, INFINITE, true);
				ProcessosAtt(false);
				TimeRefresh();
			}
		}

		System::Void OpacidadeInicio()
		{
			for (float i = 0; i < 100; i++)
			{
				this->Opacity = (float)(i / 100);
				Sleep(5);
			}
		}

		MyForm(void)
		{
			InitializeComponent();
			AjustPrivilegios();
			hGlobalEvent = CreateEventW(NULL, FALSE, FALSE, NULL);

			//ProcessosAtt(true);
			LargInicial = MyForm::Width;
			LargMax = LargInicial + (listBox2->Size.Width + 10);
			janelaAtual = (HWND)this->Handle.ToInt32();			/*HWND do form -> Usada no MessageBox*/

			MenusTimes->Add(segundosToolStripMenuItem);
			MenusTimes->Add(segundoToolStripMenuItem);
			MenusTimes->Add(segundosToolStripMenuItem1);
			MenusTimes->Add(segundosToolStripMenuItem2);

			MenusVisib->Add(toolStripMenuItem2);
			MenusVisib->Add(toolStripMenuItem3);
			MenusVisib->Add(toolStripMenuItem4);
			MenusVisib->Add(toolStripMenuItem5);
			MenusVisib->Add(toolStripMenuItem6);
			MenusVisib->Add(toolStripMenuItem7);
			MenusVisib->Add(toolStripMenuItem8);
			MenusVisib->Add(toolStripMenuItem9);
			MenusVisib->Add(toolStripMenuItem10);
			MenusVisib->Add(visívelToolStripMenuItem);

			PadraoProcessIcon = gcnew Bitmap(15, 15);

			for (int x = 0, y = 14; x < 15; x++, y--)
			{
				PadraoProcessIcon->SetPixel(x, x, Color::Red);
				PadraoProcessIcon->SetPixel(y, x, Color::Red);
			}

			for (int x = 0; x < 15; x++)
			{
				PadraoProcessIcon->SetPixel(x, 0, Color::Black);
				PadraoProcessIcon->SetPixel(x, 14, Color::Black);
			}
			for (int y = 0; y < 15; y++)
			{
				PadraoProcessIcon->SetPixel(0, y, Color::Black);
				PadraoProcessIcon->SetPixel(14, y, Color::Black);
			}

			ProcessosAtt(true);
			//EnableWindow((HWND)textBox2->Handle.ToInt32(), FALSE);

			VerificanteStrip(2);

			TemporizadorRefresh = CreateWaitableTimerA(0, TRUE, NULL); /*Temporizador da atualização dos processos, etc*/
			SleepTime = -1;

			delete[]backSlash;
			backSlash = gcnew Thread(gcnew ThreadStart(this, &MyForm::AtualizacaoProcessos));
			backSlash->Start();
			Thread^ ThreadAlertNeed = gcnew Thread(gcnew ThreadStart(this, &MyForm::ThreadAlertIn));
			ThreadAlertNeed->Start();
		}

	protected:
		~MyForm()
		{
			if (components)
			{
				delete components;
			}

		}

	private: System::Windows::Forms::Button^  button1;
	private: System::ComponentModel::IContainer^  components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->listBox2 = (gcnew System::Windows::Forms::ListBox());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column4 = (gcnew System::Windows::Forms::DataGridViewImageColumn());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column5 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->menuStrip2 = (gcnew System::Windows::Forms::MenuStrip());
			this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->opacidadeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem3 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem4 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem5 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem6 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem7 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem8 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem9 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem10 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->visívelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->atualizarToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tempoDeAtualizaçãoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->segundosToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->segundoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->segundosToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->segundosToolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->atualizaçãoAutomáticaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->sempreVisívelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ferramentasToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->calculadoraToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->leitorDeTextoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel2 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->button10 = (gcnew System::Windows::Forms::Button());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->TT_B5 = (gcnew System::Windows::Forms::Button());
			this->TT_B4 = (gcnew System::Windows::Forms::Button());
			this->TT_B3 = (gcnew System::Windows::Forms::Button());
			this->TT_B2 = (gcnew System::Windows::Forms::Button());
			this->TT_B1 = (gcnew System::Windows::Forms::Button());
			this->TT_B0 = (gcnew System::Windows::Forms::Button());
			this->dataGridView2 = (gcnew System::Windows::Forms::DataGridView());
			this->dataGridViewTextBoxColumn1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn4 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column6 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			this->menuStrip2->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView2))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->button1->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(531, 60);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(118, 23);
			this->button1->TabIndex = 4;
			this->button1->TabStop = false;
			this->button1->Text = L"Finalizar processo";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			this->button1->Enter += gcnew System::EventHandler(this, &MyForm::button5_Enter);
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->button2->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button2->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button2->Location = System::Drawing::Point(531, 0);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(118, 23);
			this->button2->TabIndex = 5;
			this->button2->TabStop = false;
			this->button2->Text = L"Atualizar";
			this->button2->UseVisualStyleBackColor = false;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			this->button2->Enter += gcnew System::EventHandler(this, &MyForm::button5_Enter);
			// 
			// button3
			// 
			this->button3->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->button3->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button3->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button3->Location = System::Drawing::Point(531, 30);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(118, 23);
			this->button3->TabIndex = 6;
			this->button3->TabStop = false;
			this->button3->Text = L"Mostrar módulos";
			this->button3->UseVisualStyleBackColor = false;
			this->button3->Click += gcnew System::EventHandler(this, &MyForm::button3_Click);
			this->button3->Enter += gcnew System::EventHandler(this, &MyForm::button5_Enter);
			// 
			// listBox2
			// 
			this->listBox2->CausesValidation = false;
			this->listBox2->FormattingEnabled = true;
			this->listBox2->Location = System::Drawing::Point(677, 47);
			this->listBox2->Name = L"listBox2";
			this->listBox2->Size = System::Drawing::Size(213, 498);
			this->listBox2->TabIndex = 7;
			this->listBox2->TabStop = false;
			this->listBox2->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::listBox2_SelectedIndexChanged);
			// 
			// button4
			// 
			this->button4->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->button4->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button4->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button4->Location = System::Drawing::Point(677, 551);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(213, 23);
			this->button4->TabIndex = 8;
			this->button4->TabStop = false;
			this->button4->Text = L"Atualizar módulos";
			this->button4->UseVisualStyleBackColor = false;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->AllowUserToResizeColumns = false;
			this->dataGridView1->AllowUserToResizeRows = false;
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
			this->dataGridView1->BackgroundColor = System::Drawing::SystemColors::Window;
			this->dataGridView1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->dataGridView1->CellBorderStyle = System::Windows::Forms::DataGridViewCellBorderStyle::None;
			this->dataGridView1->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::None;
			dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle1->BackColor = System::Drawing::SystemColors::Control;
			dataGridViewCellStyle1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			dataGridViewCellStyle1->ForeColor = System::Drawing::SystemColors::WindowText;
			dataGridViewCellStyle1->SelectionBackColor = System::Drawing::SystemColors::MenuHighlight;
			dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dataGridView1->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(5) {
				this->Column4,
					this->Column1, this->Column3, this->Column5, this->Column2
			});
			this->dataGridView1->Location = System::Drawing::Point(0, 0);
			this->dataGridView1->MultiSelect = false;
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->ReadOnly = true;
			this->dataGridView1->RowTemplate->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->dataGridView1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->ShowCellErrors = false;
			this->dataGridView1->ShowCellToolTips = false;
			this->dataGridView1->ShowEditingIcon = false;
			this->dataGridView1->ShowRowErrors = false;
			this->dataGridView1->Size = System::Drawing::Size(527, 491);
			this->dataGridView1->TabIndex = 10;
			this->dataGridView1->TabStop = false;
			this->dataGridView1->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MyForm::dataGridView1_CellContentClick);
			this->dataGridView1->SelectionChanged += gcnew System::EventHandler(this, &MyForm::dataGridView1_SelectionChanged);
			this->dataGridView1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::dataGridView1_Paint);
			this->dataGridView1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::dataGridView1_KeyDown);
			this->dataGridView1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::dataGridView1_KeyPress);
			this->dataGridView1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::dataGridView1_MouseClick);
			// 
			// Column4
			// 
			this->Column4->FillWeight = 20;
			this->Column4->HeaderText = L" ";
			this->Column4->Name = L"Column4";
			this->Column4->ReadOnly = true;
			this->Column4->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			// 
			// Column1
			// 
			this->Column1->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->Column1->FillWeight = 196.3036F;
			this->Column1->HeaderText = L"Processos";
			this->Column1->Name = L"Column1";
			this->Column1->ReadOnly = true;
			this->Column1->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->Column1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// Column3
			// 
			this->Column3->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
			this->Column3->FillWeight = 224.347F;
			this->Column3->HeaderText = L"Usuário";
			this->Column3->Name = L"Column3";
			this->Column3->ReadOnly = true;
			this->Column3->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->Column3->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column3->Width = 49;
			// 
			// Column5
			// 
			this->Column5->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
			this->Column5->FillWeight = 70;
			this->Column5->HeaderText = L"Sessão";
			this->Column5->Name = L"Column5";
			this->Column5->ReadOnly = true;
			this->Column5->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->Column5->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column5->Width = 48;
			// 
			// Column2
			// 
			this->Column2->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
			this->Column2->FillWeight = 70;
			this->Column2->HeaderText = L"PID";
			this->Column2->Name = L"Column2";
			this->Column2->ReadOnly = true;
			this->Column2->Resizable = System::Windows::Forms::DataGridViewTriState::True;
			this->Column2->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->Column2->Width = 31;
			// 
			// button5
			// 
			this->button5->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->button5->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button5->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button5->Location = System::Drawing::Point(531, 90);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(118, 23);
			this->button5->TabIndex = 11;
			this->button5->TabStop = false;
			this->button5->Text = L"Suspender";
			this->button5->UseVisualStyleBackColor = false;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm::button5_Click);
			this->button5->Enter += gcnew System::EventHandler(this, &MyForm::button5_Enter);
			// 
			// button6
			// 
			this->button6->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->button6->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button6->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button6->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button6->Location = System::Drawing::Point(531, 180);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(118, 23);
			this->button6->TabIndex = 12;
			this->button6->TabStop = false;
			this->button6->Text = L"Reduce Working Set";
			this->button6->UseVisualStyleBackColor = false;
			this->button6->Click += gcnew System::EventHandler(this, &MyForm::button6_Click);
			this->button6->Enter += gcnew System::EventHandler(this, &MyForm::button5_Enter);
			// 
			// menuStrip2
			// 
			this->menuStrip2->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->viewToolStripMenuItem,
					this->ferramentasToolStripMenuItem
			});
			this->menuStrip2->Location = System::Drawing::Point(0, 0);
			this->menuStrip2->Name = L"menuStrip2";
			this->menuStrip2->Size = System::Drawing::Size(675, 24);
			this->menuStrip2->TabIndex = 13;
			this->menuStrip2->Text = L"menuStrip2";
			// 
			// viewToolStripMenuItem
			// 
			this->viewToolStripMenuItem->AutoSize = false;
			this->viewToolStripMenuItem->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(7) {
				this->opacidadeToolStripMenuItem,
					this->toolStripSeparator1, this->atualizarToolStripMenuItem, this->tempoDeAtualizaçãoToolStripMenuItem, this->atualizaçãoAutomáticaToolStripMenuItem,
					this->toolStripSeparator2, this->sempreVisívelToolStripMenuItem
			});
			this->viewToolStripMenuItem->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->viewToolStripMenuItem->ImageTransparentColor = System::Drawing::Color::Transparent;
			this->viewToolStripMenuItem->Name = L"viewToolStripMenuItem";
			this->viewToolStripMenuItem->Size = System::Drawing::Size(94, 20);
			this->viewToolStripMenuItem->Text = L"Visualização";
			this->viewToolStripMenuItem->TextImageRelation = System::Windows::Forms::TextImageRelation::TextBeforeImage;
			// 
			// opacidadeToolStripMenuItem
			// 
			this->opacidadeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(10) {
				this->toolStripMenuItem2,
					this->toolStripMenuItem3, this->toolStripMenuItem4, this->toolStripMenuItem5, this->toolStripMenuItem6, this->toolStripMenuItem7,
					this->toolStripMenuItem8, this->toolStripMenuItem9, this->toolStripMenuItem10, this->visívelToolStripMenuItem
			});
			this->opacidadeToolStripMenuItem->Name = L"opacidadeToolStripMenuItem";
			this->opacidadeToolStripMenuItem->Size = System::Drawing::Size(217, 22);
			this->opacidadeToolStripMenuItem->Text = L"Transparência";
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->CheckOnClick = true;
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(102, 22);
			this->toolStripMenuItem2->Text = L"10%";
			this->toolStripMenuItem2->Click += gcnew System::EventHandler(this, &MyForm::toolStripMenuItem2_Click);
			// 
			// toolStripMenuItem3
			// 
			this->toolStripMenuItem3->CheckOnClick = true;
			this->toolStripMenuItem3->Name = L"toolStripMenuItem3";
			this->toolStripMenuItem3->Size = System::Drawing::Size(102, 22);
			this->toolStripMenuItem3->Text = L"20%";
			this->toolStripMenuItem3->Click += gcnew System::EventHandler(this, &MyForm::toolStripMenuItem3_Click);
			// 
			// toolStripMenuItem4
			// 
			this->toolStripMenuItem4->CheckOnClick = true;
			this->toolStripMenuItem4->Name = L"toolStripMenuItem4";
			this->toolStripMenuItem4->Size = System::Drawing::Size(102, 22);
			this->toolStripMenuItem4->Text = L"30%";
			this->toolStripMenuItem4->Click += gcnew System::EventHandler(this, &MyForm::toolStripMenuItem4_Click);
			// 
			// toolStripMenuItem5
			// 
			this->toolStripMenuItem5->CheckOnClick = true;
			this->toolStripMenuItem5->Name = L"toolStripMenuItem5";
			this->toolStripMenuItem5->Size = System::Drawing::Size(102, 22);
			this->toolStripMenuItem5->Text = L"40%";
			this->toolStripMenuItem5->Click += gcnew System::EventHandler(this, &MyForm::toolStripMenuItem5_Click);
			// 
			// toolStripMenuItem6
			// 
			this->toolStripMenuItem6->CheckOnClick = true;
			this->toolStripMenuItem6->Name = L"toolStripMenuItem6";
			this->toolStripMenuItem6->Size = System::Drawing::Size(102, 22);
			this->toolStripMenuItem6->Text = L"50%";
			this->toolStripMenuItem6->Click += gcnew System::EventHandler(this, &MyForm::toolStripMenuItem6_Click);
			// 
			// toolStripMenuItem7
			// 
			this->toolStripMenuItem7->CheckOnClick = true;
			this->toolStripMenuItem7->Name = L"toolStripMenuItem7";
			this->toolStripMenuItem7->Size = System::Drawing::Size(102, 22);
			this->toolStripMenuItem7->Text = L"60%";
			this->toolStripMenuItem7->Click += gcnew System::EventHandler(this, &MyForm::toolStripMenuItem7_Click);
			// 
			// toolStripMenuItem8
			// 
			this->toolStripMenuItem8->CheckOnClick = true;
			this->toolStripMenuItem8->Name = L"toolStripMenuItem8";
			this->toolStripMenuItem8->Size = System::Drawing::Size(102, 22);
			this->toolStripMenuItem8->Text = L"70%";
			this->toolStripMenuItem8->Click += gcnew System::EventHandler(this, &MyForm::toolStripMenuItem8_Click);
			// 
			// toolStripMenuItem9
			// 
			this->toolStripMenuItem9->CheckOnClick = true;
			this->toolStripMenuItem9->Name = L"toolStripMenuItem9";
			this->toolStripMenuItem9->Size = System::Drawing::Size(102, 22);
			this->toolStripMenuItem9->Text = L"80%";
			this->toolStripMenuItem9->Click += gcnew System::EventHandler(this, &MyForm::toolStripMenuItem9_Click);
			// 
			// toolStripMenuItem10
			// 
			this->toolStripMenuItem10->CheckOnClick = true;
			this->toolStripMenuItem10->Name = L"toolStripMenuItem10";
			this->toolStripMenuItem10->Size = System::Drawing::Size(102, 22);
			this->toolStripMenuItem10->Text = L"90%";
			this->toolStripMenuItem10->Click += gcnew System::EventHandler(this, &MyForm::toolStripMenuItem10_Click);
			// 
			// visívelToolStripMenuItem
			// 
			this->visívelToolStripMenuItem->Checked = true;
			this->visívelToolStripMenuItem->CheckOnClick = true;
			this->visívelToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->visívelToolStripMenuItem->Name = L"visívelToolStripMenuItem";
			this->visívelToolStripMenuItem->Size = System::Drawing::Size(102, 22);
			this->visívelToolStripMenuItem->Text = L"100%";
			this->visívelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::visívelToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(214, 6);
			// 
			// atualizarToolStripMenuItem
			// 
			this->atualizarToolStripMenuItem->Name = L"atualizarToolStripMenuItem";
			this->atualizarToolStripMenuItem->Size = System::Drawing::Size(217, 22);
			this->atualizarToolStripMenuItem->Text = L"Atualizar";
			this->atualizarToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::atualizarToolStripMenuItem_Click);
			// 
			// tempoDeAtualizaçãoToolStripMenuItem
			// 
			this->tempoDeAtualizaçãoToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->segundosToolStripMenuItem,
					this->segundoToolStripMenuItem, this->segundosToolStripMenuItem1, this->segundosToolStripMenuItem2
			});
			this->tempoDeAtualizaçãoToolStripMenuItem->Name = L"tempoDeAtualizaçãoToolStripMenuItem";
			this->tempoDeAtualizaçãoToolStripMenuItem->Size = System::Drawing::Size(217, 22);
			this->tempoDeAtualizaçãoToolStripMenuItem->Text = L"Tempo de atualização";
			// 
			// segundosToolStripMenuItem
			// 
			this->segundosToolStripMenuItem->CheckOnClick = true;
			this->segundosToolStripMenuItem->Name = L"segundosToolStripMenuItem";
			this->segundosToolStripMenuItem->Size = System::Drawing::Size(140, 22);
			this->segundosToolStripMenuItem->Text = L"1/2 segundo";
			this->segundosToolStripMenuItem->CheckedChanged += gcnew System::EventHandler(this, &MyForm::segundosToolStripMenuItem_CheckedChanged);
			// 
			// segundoToolStripMenuItem
			// 
			this->segundoToolStripMenuItem->Checked = true;
			this->segundoToolStripMenuItem->CheckOnClick = true;
			this->segundoToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->segundoToolStripMenuItem->Name = L"segundoToolStripMenuItem";
			this->segundoToolStripMenuItem->Size = System::Drawing::Size(140, 22);
			this->segundoToolStripMenuItem->Text = L"1 segundo";
			this->segundoToolStripMenuItem->CheckedChanged += gcnew System::EventHandler(this, &MyForm::segundoToolStripMenuItem_CheckedChanged);
			this->segundoToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::segundoToolStripMenuItem_Click);
			// 
			// segundosToolStripMenuItem1
			// 
			this->segundosToolStripMenuItem1->CheckOnClick = true;
			this->segundosToolStripMenuItem1->Name = L"segundosToolStripMenuItem1";
			this->segundosToolStripMenuItem1->Size = System::Drawing::Size(140, 22);
			this->segundosToolStripMenuItem1->Text = L"5 segundos";
			this->segundosToolStripMenuItem1->CheckedChanged += gcnew System::EventHandler(this, &MyForm::segundosToolStripMenuItem1_CheckedChanged);
			// 
			// segundosToolStripMenuItem2
			// 
			this->segundosToolStripMenuItem2->CheckOnClick = true;
			this->segundosToolStripMenuItem2->Name = L"segundosToolStripMenuItem2";
			this->segundosToolStripMenuItem2->Size = System::Drawing::Size(140, 22);
			this->segundosToolStripMenuItem2->Text = L"10 segundos";
			this->segundosToolStripMenuItem2->CheckedChanged += gcnew System::EventHandler(this, &MyForm::segundosToolStripMenuItem2_CheckedChanged);
			// 
			// atualizaçãoAutomáticaToolStripMenuItem
			// 
			this->atualizaçãoAutomáticaToolStripMenuItem->Checked = true;
			this->atualizaçãoAutomáticaToolStripMenuItem->CheckOnClick = true;
			this->atualizaçãoAutomáticaToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->atualizaçãoAutomáticaToolStripMenuItem->Name = L"atualizaçãoAutomáticaToolStripMenuItem";
			this->atualizaçãoAutomáticaToolStripMenuItem->Size = System::Drawing::Size(217, 22);
			this->atualizaçãoAutomáticaToolStripMenuItem->Text = L"Atualizar automaticamente";
			this->atualizaçãoAutomáticaToolStripMenuItem->CheckedChanged += gcnew System::EventHandler(this, &MyForm::atualizaçãoAutomáticaToolStripMenuItem_CheckedChanged);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(214, 6);
			// 
			// sempreVisívelToolStripMenuItem
			// 
			this->sempreVisívelToolStripMenuItem->CheckOnClick = true;
			this->sempreVisívelToolStripMenuItem->Name = L"sempreVisívelToolStripMenuItem";
			this->sempreVisívelToolStripMenuItem->Size = System::Drawing::Size(217, 22);
			this->sempreVisívelToolStripMenuItem->Text = L"Sempre visível";
			this->sempreVisívelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::sempreVisívelToolStripMenuItem_Click);
			// 
			// ferramentasToolStripMenuItem
			// 
			this->ferramentasToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->calculadoraToolStripMenuItem,
					this->leitorDeTextoToolStripMenuItem
			});
			this->ferramentasToolStripMenuItem->Name = L"ferramentasToolStripMenuItem";
			this->ferramentasToolStripMenuItem->Size = System::Drawing::Size(84, 20);
			this->ferramentasToolStripMenuItem->Text = L"Ferramentas";
			// 
			// calculadoraToolStripMenuItem
			// 
			this->calculadoraToolStripMenuItem->Name = L"calculadoraToolStripMenuItem";
			this->calculadoraToolStripMenuItem->Size = System::Drawing::Size(149, 22);
			this->calculadoraToolStripMenuItem->Text = L"Calculadora";
			this->calculadoraToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::calculadoraToolStripMenuItem_Click);
			// 
			// leitorDeTextoToolStripMenuItem
			// 
			this->leitorDeTextoToolStripMenuItem->Name = L"leitorDeTextoToolStripMenuItem";
			this->leitorDeTextoToolStripMenuItem->Size = System::Drawing::Size(149, 22);
			this->leitorDeTextoToolStripMenuItem->Text = L"Leitor de texto";
			this->leitorDeTextoToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::leitorDeTextoToolStripMenuItem_Click);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(61, 4);
			// 
			// button8
			// 
			this->button8->AutoSize = true;
			this->button8->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->button8->Enabled = false;
			this->button8->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button8->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button8->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button8->ForeColor = System::Drawing::Color::Black;
			this->button8->Location = System::Drawing::Point(677, 577);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(105, 26);
			this->button8->TabIndex = 18;
			this->button8->TabStop = false;
			this->button8->Text = L"Abrir local";
			this->button8->UseVisualStyleBackColor = false;
			this->button8->Click += gcnew System::EventHandler(this, &MyForm::button8_Click);
			// 
			// button7
			// 
			this->button7->AutoSize = true;
			this->button7->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->button7->Enabled = false;
			this->button7->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button7->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button7->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button7->Location = System::Drawing::Point(785, 577);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(105, 26);
			this->button7->TabIndex = 19;
			this->button7->TabStop = false;
			this->button7->Text = L"Propriedades";
			this->button7->UseVisualStyleBackColor = false;
			this->button7->Click += gcnew System::EventHandler(this, &MyForm::button7_Click_1);
			// 
			// comboBox1
			// 
			this->comboBox1->BackColor = System::Drawing::SystemColors::Window;
			this->comboBox1->CausesValidation = false;
			this->comboBox1->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->comboBox1->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->comboBox1->ForeColor = System::Drawing::Color::Black;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(12, 578);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(658, 24);
			this->comboBox1->TabIndex = 20;
			this->comboBox1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::comboBox1_KeyPress);
			// 
			// button9
			// 
			this->button9->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->button9->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button9->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button9->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button9->Location = System::Drawing::Point(531, 150);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(118, 23);
			this->button9->TabIndex = 21;
			this->button9->TabStop = false;
			this->button9->Text = L"Handles";
			this->button9->UseVisualStyleBackColor = false;
			this->button9->Click += gcnew System::EventHandler(this, &MyForm::button9_Click);
			this->button9->Enter += gcnew System::EventHandler(this, &MyForm::button5_Enter);
			// 
			// statusStrip1
			// 
			this->statusStrip1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->toolStripStatusLabel1,
					this->toolStripStatusLabel2
			});
			this->statusStrip1->Location = System::Drawing::Point(0, 606);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->statusStrip1->Size = System::Drawing::Size(675, 22);
			this->statusStrip1->SizingGrip = false;
			this->statusStrip1->TabIndex = 22;
			this->statusStrip1->Text = L"eae";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(71, 17);
			this->toolStripStatusLabel1->Text = L"Processos: 0";
			// 
			// toolStripStatusLabel2
			// 
			this->toolStripStatusLabel2->Name = L"toolStripStatusLabel2";
			this->toolStripStatusLabel2->Padding = System::Windows::Forms::Padding(50, 0, 0, 0);
			this->toolStripStatusLabel2->Size = System::Drawing::Size(100, 17);
			this->toolStripStatusLabel2->Text = L"Horário:";
			// 
			// button10
			// 
			this->button10->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->button10->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button10->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button10->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button10->Location = System::Drawing::Point(531, 120);
			this->button10->Name = L"button10";
			this->button10->Size = System::Drawing::Size(118, 23);
			this->button10->TabIndex = 30;
			this->button10->TabStop = false;
			this->button10->Text = L"Reiniciar processo";
			this->button10->UseVisualStyleBackColor = false;
			this->button10->Click += gcnew System::EventHandler(this, &MyForm::button10_Click);
			this->button10->Enter += gcnew System::EventHandler(this, &MyForm::button5_Enter);
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Location = System::Drawing::Point(11, 27);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(660, 518);
			this->tabControl1->TabIndex = 31;
			this->tabControl1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::tabControl1_SelectedIndexChanged);
			// 
			// tabPage1
			// 
			this->tabPage1->BackColor = System::Drawing::Color::White;
			this->tabPage1->Controls->Add(this->dataGridView1);
			this->tabPage1->Controls->Add(this->button2);
			this->tabPage1->Controls->Add(this->button10);
			this->tabPage1->Controls->Add(this->button3);
			this->tabPage1->Controls->Add(this->button1);
			this->tabPage1->Controls->Add(this->button5);
			this->tabPage1->Controls->Add(this->button6);
			this->tabPage1->Controls->Add(this->button9);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(652, 492);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Processos";
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->TT_B5);
			this->tabPage2->Controls->Add(this->TT_B4);
			this->tabPage2->Controls->Add(this->TT_B3);
			this->tabPage2->Controls->Add(this->TT_B2);
			this->tabPage2->Controls->Add(this->TT_B1);
			this->tabPage2->Controls->Add(this->TT_B0);
			this->tabPage2->Controls->Add(this->dataGridView2);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(652, 492);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Serviços";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// TT_B5
			// 
			this->TT_B5->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->TT_B5->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->TT_B5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->TT_B5->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TT_B5->Location = System::Drawing::Point(531, 149);
			this->TT_B5->Name = L"TT_B5";
			this->TT_B5->Size = System::Drawing::Size(118, 23);
			this->TT_B5->TabIndex = 37;
			this->TT_B5->TabStop = false;
			this->TT_B5->Text = L"Deletar serviço";
			this->TT_B5->UseVisualStyleBackColor = false;
			this->TT_B5->Click += gcnew System::EventHandler(this, &MyForm::DeleteTheService);
			// 
			// TT_B4
			// 
			this->TT_B4->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->TT_B4->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->TT_B4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->TT_B4->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TT_B4->Location = System::Drawing::Point(531, 120);
			this->TT_B4->Name = L"TT_B4";
			this->TT_B4->Size = System::Drawing::Size(118, 23);
			this->TT_B4->TabIndex = 36;
			this->TT_B4->TabStop = false;
			this->TT_B4->Text = L"Parar serviço";
			this->TT_B4->UseVisualStyleBackColor = false;
			this->TT_B4->Click += gcnew System::EventHandler(this, &MyForm::StopTheService);
			// 
			// TT_B3
			// 
			this->TT_B3->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->TT_B3->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->TT_B3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->TT_B3->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TT_B3->Location = System::Drawing::Point(531, 90);
			this->TT_B3->Name = L"TT_B3";
			this->TT_B3->Size = System::Drawing::Size(118, 23);
			this->TT_B3->TabIndex = 35;
			this->TT_B3->TabStop = false;
			this->TT_B3->Text = L"Reiniciar serviço";
			this->TT_B3->UseVisualStyleBackColor = false;
			this->TT_B3->Click += gcnew System::EventHandler(this, &MyForm::RestartTheService);
			// 
			// TT_B2
			// 
			this->TT_B2->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->TT_B2->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->TT_B2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->TT_B2->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TT_B2->Location = System::Drawing::Point(531, 60);
			this->TT_B2->Name = L"TT_B2";
			this->TT_B2->Size = System::Drawing::Size(118, 23);
			this->TT_B2->TabIndex = 34;
			this->TT_B2->TabStop = false;
			this->TT_B2->Text = L"Iniciar serviço";
			this->TT_B2->UseVisualStyleBackColor = false;
			this->TT_B2->Click += gcnew System::EventHandler(this, &MyForm::StartTheService);
			// 
			// TT_B1
			// 
			this->TT_B1->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->TT_B1->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->TT_B1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->TT_B1->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TT_B1->Location = System::Drawing::Point(531, 30);
			this->TT_B1->Name = L"TT_B1";
			this->TT_B1->Size = System::Drawing::Size(118, 23);
			this->TT_B1->TabIndex = 33;
			this->TT_B1->TabStop = false;
			this->TT_B1->Text = L"Ir para processo";
			this->TT_B1->UseVisualStyleBackColor = false;
			this->TT_B1->Click += gcnew System::EventHandler(this, &MyForm::GoToProcess);
			// 
			// TT_B0
			// 
			this->TT_B0->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->TT_B0->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->TT_B0->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->TT_B0->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TT_B0->Location = System::Drawing::Point(531, 0);
			this->TT_B0->Name = L"TT_B0";
			this->TT_B0->Size = System::Drawing::Size(118, 23);
			this->TT_B0->TabIndex = 31;
			this->TT_B0->TabStop = false;
			this->TT_B0->Text = L"Atualizar";
			this->TT_B0->UseVisualStyleBackColor = false;
			this->TT_B0->Click += gcnew System::EventHandler(this, &MyForm::ServicesRefresh);
			// 
			// dataGridView2
			// 
			this->dataGridView2->AllowUserToAddRows = false;
			this->dataGridView2->AllowUserToDeleteRows = false;
			this->dataGridView2->AllowUserToResizeColumns = false;
			this->dataGridView2->AllowUserToResizeRows = false;
			this->dataGridView2->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
			this->dataGridView2->BackgroundColor = System::Drawing::SystemColors::Window;
			this->dataGridView2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->dataGridView2->CellBorderStyle = System::Windows::Forms::DataGridViewCellBorderStyle::None;
			this->dataGridView2->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::None;
			dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle2->BackColor = System::Drawing::SystemColors::Control;
			dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			dataGridViewCellStyle2->ForeColor = System::Drawing::SystemColors::WindowText;
			dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::MenuHighlight;
			dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dataGridView2->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
			this->dataGridView2->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
			this->dataGridView2->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(5) {
				this->dataGridViewTextBoxColumn1,
					this->dataGridViewTextBoxColumn2, this->dataGridViewTextBoxColumn3, this->dataGridViewTextBoxColumn4, this->Column6
			});
			this->dataGridView2->Location = System::Drawing::Point(0, 0);
			this->dataGridView2->MultiSelect = false;
			this->dataGridView2->Name = L"dataGridView2";
			this->dataGridView2->RowHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
			this->dataGridView2->RowHeadersVisible = false;
			this->dataGridView2->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
			this->dataGridView2->RowTemplate->ReadOnly = true;
			this->dataGridView2->RowTemplate->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->dataGridView2->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->dataGridView2->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView2->ShowCellErrors = false;
			this->dataGridView2->ShowCellToolTips = false;
			this->dataGridView2->ShowEditingIcon = false;
			this->dataGridView2->ShowRowErrors = false;
			this->dataGridView2->Size = System::Drawing::Size(527, 491);
			this->dataGridView2->TabIndex = 32;
			this->dataGridView2->TabStop = false;
			this->dataGridView2->SelectionChanged += gcnew System::EventHandler(this, &MyForm::dataGridView2_SelectionChanged);
			// 
			// dataGridViewTextBoxColumn1
			// 
			this->dataGridViewTextBoxColumn1->HeaderText = L"Nome";
			this->dataGridViewTextBoxColumn1->Name = L"dataGridViewTextBoxColumn1";
			this->dataGridViewTextBoxColumn1->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->dataGridViewTextBoxColumn1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// dataGridViewTextBoxColumn2
			// 
			this->dataGridViewTextBoxColumn2->FillWeight = 150;
			this->dataGridViewTextBoxColumn2->HeaderText = L"Display Name";
			this->dataGridViewTextBoxColumn2->Name = L"dataGridViewTextBoxColumn2";
			this->dataGridViewTextBoxColumn2->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->dataGridViewTextBoxColumn2->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// dataGridViewTextBoxColumn3
			// 
			this->dataGridViewTextBoxColumn3->FillWeight = 130.1022F;
			this->dataGridViewTextBoxColumn3->HeaderText = L"Tipo";
			this->dataGridViewTextBoxColumn3->Name = L"dataGridViewTextBoxColumn3";
			this->dataGridViewTextBoxColumn3->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->dataGridViewTextBoxColumn3->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// dataGridViewTextBoxColumn4
			// 
			this->dataGridViewTextBoxColumn4->FillWeight = 150;
			this->dataGridViewTextBoxColumn4->HeaderText = L"Estado";
			this->dataGridViewTextBoxColumn4->Name = L"dataGridViewTextBoxColumn4";
			this->dataGridViewTextBoxColumn4->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->dataGridViewTextBoxColumn4->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// Column6
			// 
			this->Column6->FillWeight = 50;
			this->Column6->HeaderText = L"PID";
			this->Column6->Name = L"Column6";
			this->Column6->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->Column6->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// label1
			// 
			this->label1->BackColor = System::Drawing::SystemColors::Highlight;
			this->label1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->label1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->label1->Font = (gcnew System::Drawing::Font(L"Arial", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(11, 551);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(659, 23);
			this->label1->TabIndex = 32;
			this->label1->Text = L"Executar uma nova tarefa";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(675, 628);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->button8);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->listBox2);
			this->Controls->Add(this->menuStrip2);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->ImeMode = System::Windows::Forms::ImeMode::Off;
			this->MaximizeBox = false;
			this->Name = L"MyForm";
			this->Text = L"Gerenciador de tarefas";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyDown);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			this->menuStrip2->ResumeLayout(false);
			this->menuStrip2->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

#pragma endregion

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

		std::wstring ProcessoAtual;
		System::String ^ ProcActual = dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column1"]->Value->ToString();
		int PID_ALVO = Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value->ToString());

		for (int i = 0; i < ProcActual->Length; i++)
			ProcessoAtual += ProcActual[i];

		wstring MensagemDispatched = L"Tem certeza que deseja finalizar \"" + ProcessoAtual + L"\"?";
		wstring TitleDispatched = L"Erro";
		DWORD IconMessagemDispathed = MB_YESNO;
		
		HANDLE hProcesso = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID_ALVO);
		ULONG processCrit = 0x0;
		NtQueryInformationProcess(hProcesso, (PROCESSINFOCLASS)29, (PVOID)&processCrit, sizeof((PROCESSINFOCLASS)29), NULL);

		if (processCrit != 0l){/*Processo crítico do sistema*/
			IconMessagemDispathed |= MB_ICONWARNING; /* |= mesma coisa que +=*/
			MensagemDispatched = L"Este é um processo crítico e reservado pelo sistema."
				L"\nA finalização deste processo resultará em desligamento do PC.\n" + MensagemDispatched;
			TitleDispatched = L"Atenção";
		}

		int MensagemDispatchedFinal = MessageBoxW(janelaAtual, MensagemDispatched.c_str(), TitleDispatched.c_str(), IconMessagemDispathed);

		if (MensagemDispatchedFinal == IDYES)
		{
			CloseHandle(hProcesso);
			hProcesso = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION, FALSE, PID_ALVO);

			if (hProcesso == INVALID_HANDLE_VALUE || GetLastError() == ERROR_ACCESS_DENIED)
			{
				CloseHandle(hProcesso);
				MensagemDispathError(0);
			}
			else{
				TerminateProcess(hProcesso, 0);
				CloseHandle(hProcesso);
			}
		}

	}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		ProcessosAtt(false);
	}

			 System::Void MoverLargJanela()
			 {
				 if (ToLeftOrRight)
				 {
					 for (int i = MyForm::Width; i <= LargMax && ToLeftOrRight; i = i + 3)
						 MyForm::Width = i;
					 ExitThread(EXIT_SUCCESS);
				 }
				 else{
					 for (int i = MyForm::Width; i >= LargInicial && !ToLeftOrRight; i = i - 3)
						 MyForm::Width = i;
					 ExitThread(EXIT_SUCCESS);
				 }
			 }

	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
		if (dataGridView1->SelectedRows[0]->Visible == true && dataGridView1->Rows[dataGridView1->SelectedRows[0]->Index]->DefaultCellStyle->BackColor == Color::Empty)
		{
			int PID_ALVO = Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value->ToString());
			ToLeftOrRight = !ToLeftOrRight;
			Thread^ MovWin = gcnew Thread(gcnew ThreadStart(this, &MyForm::MoverLargJanela));
			MovWin->Start();
			if (ToLeftOrRight)
			{
				button3->Text = "Ocultar módulos";
				button3->Enabled = false;
				
				pidToModule = PID_ALVO;
				SetEvent(hGlobalEvent);

				button3->Enabled = true;
			}
			else
				button3->Text = "Mostrar módulos";
		}
	}

	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
		if (dataGridView1->SelectedRows[0]->Visible == true && dataGridView1->Rows[dataGridView1->SelectedRows[0]->Index]->DefaultCellStyle->BackColor == Color::Empty)
		{
			lastPid = -1;
			int PID_ALVO = Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value->ToString());

			pidToModule = PID_ALVO;
			SetEvent(hGlobalEvent);
		}
	}
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	}

			 bool *SuspendResume = new bool[2048]{false};
			 bool SusOrRes = false;

			 List<System::Int32>^ ProcessesSuspended = gcnew List < System::Int32 >;

			 bool CheckProcessExistense(DWORD pid)
			 {
				 PROCESSENTRY32W procEntry;
				 procEntry.dwSize = sizeof(PROCESSENTRY32W);

				 HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
				 Process32FirstW(hSnapshot, &procEntry);
				 do{
					 if (procEntry.th32ProcessID == pid)
					 {
						 CloseHandle(hSnapshot);
						 return true;
					 }
				 } while (Process32NextW(hSnapshot, &procEntry));
				 CloseHandle(hSnapshot);
				 return false;
			 }

	private: System::Void dataGridView1_SelectionChanged(System::Object^  sender, System::EventArgs^  e) {		
		if (dataGridView1->SelectedRows[0]->Visible == true)
		{
			DWORD pidSelecionado = Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value->ToString());
			if (ProcessesSuspended->Contains(pidSelecionado))
				button5->Text = "Resumir";
			else
				button5->Text = "Suspender";

			if (ToLeftOrRight)
			{
				pidToModule = pidSelecionado;
				SetEvent(hGlobalEvent);
			}
		}
	}

	private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
		
		if (dataGridView1->SelectedRows[0]->Visible == true)
		{
			HANDLE hProcess = OpenProcess(PROCESS_SUSPEND_RESUME, 0, Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value));

			if (hProcess == INVALID_HANDLE_VALUE || GetLastError() == ERROR_ACCESS_DENIED)
			{
				MensagemDispathError(0);
				return;
			}

			if (!ProcessesSuspended->Contains(Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value->ToString())))
			{
				ProcessesSuspended->Add(Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value->ToString()));
				dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->DefaultCellStyle->BackColor = Color::Gray;
				NtSuspendProcess(hProcess);
				button5->Text = "Resumir";
			}

			else
			{
				button5->Text = "Suspender";
				for (int i = 0; i < ProcessesSuspended->Count; i++)
				{

					if (ProcessesSuspended[i] == Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value->ToString()))
					{
						ProcessesSuspended->RemoveAt(i);
						break;
					}
				}

				dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->DefaultCellStyle->BackColor = Color::Empty;
				NtResumeProcess(hProcess);
			}

			CloseHandle(hProcess);
		}
	}
	private: System::Void dataGridView1_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {

	}
	private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {

		if (dataGridView1->SelectedRows[0]->Visible == true && dataGridView1->Rows[dataGridView1->SelectedRows[0]->Index]->DefaultCellStyle->BackColor == Color::Empty)
		{
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_SET_QUOTA, 0, Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value));
			if (hProcess == INVALID_HANDLE_VALUE || GetLastError() == ERROR_ACCESS_DENIED)
			{
				MensagemDispathError(0);
				return;
			}
			DWORD lastError = 0;
			if (K32EmptyWorkingSet(hProcess) == 0)
			{
				lastError = GetLastError();
				MensagemDispathError(0);
			}
		}
		
	}
 /*buttons->Add(segundosToolStripMenuItem);
 buttons->Add(segundoToolStripMenuItem);
 buttons->Add(segundosToolStripMenuItem1);
 buttons->Add(segundosToolStripMenuItem2);*/


			 DWORD findpid(std::wstring processo)
			 {
				 PROCESSENTRY32W procEntry;
				 procEntry.dwSize = sizeof(PROCESSENTRY32W);

				 std::wstring Processo;
				 transform(processo.begin(), processo.end(), processo.begin(), toupper);

				 HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
				 Process32FirstW(hSnapshot, &procEntry);
				 do{
					 Processo = procEntry.szExeFile;
					 transform(Processo.begin(), Processo.end(), Processo.begin(), toupper);

					 if (Processo == processo)
					 {
						 NtClose(hSnapshot);
						 return procEntry.th32ProcessID;
					 }
				 } while (Process32NextW(hSnapshot, &procEntry));

				 NtClose(hSnapshot);
				 return 0;
			 }

			 void VerificanteStrip(int Bypass)
			 {
				 for (int i = 0; i < 4; i++)
				 {
					 if (i != (Bypass - 1))
						 MenusTimes[i]->Checked = false;
				 }
			 }

	private: System::Void segundoToolStripMenuItem_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

		if (segundoToolStripMenuItem->Checked == true)
		{
			VerificanteStrip(2);
			SleepTime = -1;
		}
	}
	private: System::Void segundosToolStripMenuItem_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		if (segundosToolStripMenuItem->Checked == true)
		{
			VerificanteStrip(1);
			SleepTime = 0.5;
		}
	}
	private: System::Void segundosToolStripMenuItem1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		if (segundosToolStripMenuItem1->Checked == true)
		{
			VerificanteStrip(3);
			SleepTime = -5;
		}
	}
	private: System::Void segundosToolStripMenuItem2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		if (segundosToolStripMenuItem2->Checked == true)
		{
			VerificanteStrip(4);
			SleepTime = -10;
		}
	}
	private: System::Void atualizaçãoAutomáticaToolStripMenuItem_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		CheckedAtt = atualizaçãoAutomáticaToolStripMenuItem->Checked;
		if (atualizaçãoAutomáticaToolStripMenuItem->Checked == true)
		{
			delete[]backSlash;
			backSlash = gcnew Thread(gcnew ThreadStart(this, &MyForm::AtualizacaoProcessos));
			backSlash->Start();
		}
	}

			 void TrocarVisibilidade(float Identificador)
			 {
				 for (int i = 0; i < 10; i++)
				 {
					 if (i != (int)(Identificador - 1))
						 MenusVisib[i]->Checked = false;
				 }

				 this->Opacity = (Identificador * 10) / 100;
			 }

	private: System::Void visívelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		//100%
		TrocarVisibilidade(10);
	}
	private: System::Void atualizarToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		ProcessosAtt(false);
	}
	private: System::Void toolStripMenuItem2_Click(System::Object^  sender, System::EventArgs^  e) {
		//10%
		TrocarVisibilidade(1);
	}
	private: System::Void toolStripMenuItem3_Click(System::Object^  sender, System::EventArgs^  e) {
		TrocarVisibilidade(2);
	}
	private: System::Void toolStripMenuItem4_Click(System::Object^  sender, System::EventArgs^  e) {
		TrocarVisibilidade(3);
	}
	private: System::Void toolStripMenuItem5_Click(System::Object^  sender, System::EventArgs^  e) {
		TrocarVisibilidade(4);
	}
	private: System::Void toolStripMenuItem6_Click(System::Object^  sender, System::EventArgs^  e) {
		TrocarVisibilidade(5);
	}
	private: System::Void toolStripMenuItem7_Click(System::Object^  sender, System::EventArgs^  e) {
		TrocarVisibilidade(6);
	}
	private: System::Void toolStripMenuItem8_Click(System::Object^  sender, System::EventArgs^  e) {
		TrocarVisibilidade(7);
	}
	private: System::Void toolStripMenuItem9_Click(System::Object^  sender, System::EventArgs^  e) {
		TrocarVisibilidade(8);
	}
	private: System::Void toolStripMenuItem10_Click(System::Object^  sender, System::EventArgs^  e) {
		TrocarVisibilidade(9);
	}
	private: System::Void sempreVisívelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		this->TopMost = sempreVisívelToolStripMenuItem->Checked;
	}
	private: System::Void segundoToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void textBox1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {

	}

	private: System::Void dataGridView1_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
		if (e->KeyValue == 46)	//Valor da tecla delete
		{
			button1->PerformClick(); //simula um clique no botão
		}

	}
	private: System::Void MyForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {

	}

	private: System::Void button5_Enter(System::Object^  sender, System::EventArgs^  e) {
		dataGridView1->Focus();
	}

	private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

	}

	private: System::Void dataGridView1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
	}

	private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e) {
		if (dataGridView1->SelectedRows[0]->Visible == true)
		{
			DWORD processoAlvo = Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value->ToString());
			HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, processoAlvo);

			if (!CheckProcessExistense(processoAlvo))	/*Verificar se o processo realmente existe*/
			{
				CloseHandle(hProc);			/*Fechar o identificador na memória*/
				button4->PerformClick();	/*Simular um clique no botão 4, que é para atualizar os módulos*/
				MensagemDispathError(0);
				//MessageBoxW(janelaAtual, std::wstring(L"O processo não encontrado.").c_str(), 0, MB_ICONERROR);
				return;
			}
			wchar_t ModulePathFile[MAX_PATH];

			System::String^ ModuleDisp = listBox2->Items[listBox2->SelectedIndex]->ToString();
			System::String^ ModuleDispCheck;

			for (int Master = 0; hModulosPRE[Master] != NULL; Master++)
			{
				wchar_t ModuleNameT_[MAX_PATH];
				GetModuleBaseNameW(hProc, hModulosPRE[Master], ModuleNameT_, MAX_PATH);
				ModuleDispCheck = gcnew System::String(ModuleNameT_);
				if (ModuleDisp == ModuleDispCheck)
				{
					GetModuleFileNameExW(hProc, hModulosPRE[Master], ModulePathFile, MAX_PATH);
					CloseHandle(hProc);
					break;
				}
			}
			delete[] ModuleDisp;
			delete[] ModuleDispCheck;

			CoInitialize(NULL);					//Inicializador do objeto COM -> Necessário porque o explorer usa objetos COM
			ITEMIDLIST *ArquivoSelect = ILCreateFromPath(ModulePathFile);	//Selecionar o arquivo pelo explorer

			DWORD CheckArquivoExist = NULL;
			CheckArquivoExist = GetFileAttributesW(ModulePathFile);
			if (ArquivoSelect && CheckArquivoExist != INVALID_FILE_ATTRIBUTES)
			{
				if (SHOpenFolderAndSelectItems(ArquivoSelect, 0, 0, 0) != S_OK)
					//MessageBoxW(janelaAtual, std::wstring(L"O arquivo: " + (std::wstring)ModulePathFile + L". Não pôde ser encontrado.").c_str(), 0, MB_ICONERROR);
					MensagemDispathError(0);
			}
			else
			{
				//MessageBoxW(janelaAtual, std::wstring(L"O arquivo: " + (std::wstring)ModulePathFile + L". Não pôde ser encontrado.").c_str(), 0, MB_ICONERROR);
				if (GetLastError() == 0)
					MensagemDispathError(ERROR_FILE_NOT_FOUND);
				else
					MensagemDispathError(0);
			}

			ILFree(ArquivoSelect);											//Remove o objeto da memória
			CoUninitialize();												//Término COM
			CloseHandle(hProc);
		}
	}
	private: System::Void button7_Click_1(System::Object^  sender, System::EventArgs^  e) {
		
		if (dataGridView1->SelectedRows[0]->Visible == true && dataGridView1->Rows[dataGridView1->SelectedRows[0]->Index]->DefaultCellStyle->BackColor == Color::Empty)
		{
			DWORD processoAlvo = Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value->ToString());
			HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, processoAlvo);

			if (!CheckProcessExistense(processoAlvo))
			{
				CloseHandle(hProc);
				button4->PerformClick();
				//MessageBoxW(janelaAtual, std::wstring(L"O processo não encontrado.").c_str(), 0, MB_ICONERROR);
				MensagemDispathError(0);
				return;
			}
			wchar_t ModulePathFile[MAX_PATH];

			SHELLEXECUTEINFOW ShellInfo = { 0 };
			System::String^ ModuleDisp = listBox2->Items[listBox2->SelectedIndex]->ToString();
			System::String^ ModuleDispCheck;

			for (int Master = 0; hModulosPRE[Master] != NULL; Master++)
			{
				wchar_t ModuleNameT_[MAX_PATH];
				GetModuleBaseNameW(hProc, hModulosPRE[Master], ModuleNameT_, MAX_PATH);
				ModuleDispCheck = gcnew System::String(ModuleNameT_);
				if (ModuleDisp == ModuleDispCheck)
				{
					GetModuleFileNameExW(hProc, hModulosPRE[Master], ModulePathFile, MAX_PATH);
					CloseHandle(hProc);
					break;
				}
			}
			delete[] ModuleDisp;
			delete[] ModuleDispCheck;

			DWORD CheckArquivoExist = NULL;
			CheckArquivoExist = GetFileAttributesW(ModulePathFile);

			if (CheckArquivoExist != INVALID_FILE_ATTRIBUTES)
			{
				ShellInfo.cbSize = sizeof(ShellInfo);		/*TAMANHO DA ESTRUTURA*/
				ShellInfo.lpFile = ModulePathFile;			/*LOCAL DO ARQUIVO*/
				ShellInfo.nShow = SW_SHOW;					/*SW_SHOW = JANELA VISÍVEL*/
				ShellInfo.fMask = SEE_MASK_INVOKEIDLIST;
				ShellInfo.lpVerb = L"properties";			/*TIPO DE AÇÃO -> NO CASO A FUNÇÃO SHELLEXECUTA VAI EXIBIR AS PROPRIEDADE DO ARQUIVO*/
				ShellExecuteExW(&ShellInfo);				/*INICIAR A FUNÇÃO DE ACORDO COM A ESTRUTURA DEFINIDA PREVIAMENTE (ANTES)*/
			}
			else
			{
				//MessageBoxW(janelaAtual, std::wstring(L"O arquivo: " + (std::wstring)ModulePathFile + L". Não pôde ser encontrado.").c_str(), 0, MB_ICONERROR);
				if (GetLastError() == 0)
					MensagemDispathError(ERROR_FILE_NOT_FOUND);
				else
					MensagemDispathError(0);
			}

			CloseHandle(hProc);
		}

	}
	private: System::Void listBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		bool EnableOrDisable = false;

		if (listBox2->SelectedIndex > -1 && listBox2->Items[0]->ToString() != "Acesso negado.")
			EnableOrDisable = true;
		else
			EnableOrDisable = false;

		button7->Enabled = EnableOrDisable;
		button8->Enabled = EnableOrDisable;
	}

private: System::Void comboBox1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
	if (e->KeyChar == 13)
	{
		std::wstring StrText;
			for (int i = 0; i < comboBox1->Text->Length; i++)
			StrText += (wchar_t)comboBox1->Text[i];
			while (StrText[0] == ' ') /*Remove todos os primeiros caracteres se eles forem ' ' (espaço em branco)*/
			StrText.erase(0, 1); /*primeiro parâmetro(posição), segundo parâmetro(quantidade de caracteres a ser removido)*/
			while (StrText.find('"') != std::string::npos) /*Remove todas as aspas '"'*/
			StrText.erase(StrText.find_first_of('"'), 1);
			std::wstring programa, linhadecomando;
			BOOL isPathLocation = PathIsDirectoryW(StrText.c_str()); /*Checar se o texto escrito é um diretório*/
			std::size_t	 divisor = 0;
			std::size_t	 divisob = 0;
			if (isPathLocation == FALSE)
			{
			std::wstring tempcheck;
			TCHAR checkhelp[MAX_PATH];
			size_t checkNumberH = 0l;	//0l = nullptr (null pointer) || 0 em long;
			checkNumberH = StrText.find_first_of(' ');
			BOOL cmdLine__ = FALSE;		//checar linhar de comando
			
			if (checkNumberH > 0 && checkNumberH < StrText.length())
			/*Verificar se o espaço em branco(linha de comando) está dentro do texto*/
			{
			tempcheck = StrText.substr(0, checkNumberH);/*Executável até a onde começa a linha de comando*/
			cmdLine__ = TRUE;
			}
			else
			{
				cmdLine__ = FALSE;
				tempcheck = StrText;
			}

			long checkResult = (long)FindExecutableW(tempcheck.c_str(), NULL, checkhelp);
			/*Verifica se o sistema consegue encontrar algum arquivo relativo*/
			/*Caso o número de retorno da função FindExecutable for acima de 32 indica sucesso. (de acordo com a documentação)*/

			if (StrText.find('\\') != std::string::npos && checkResult <= 32)
			{
				/*Pegando o diretório para o programa*/
				for (int i = 0; i < StrText.length(); i++)
				{
					if (StrText[i] == '\\')
						divisor = i;
				/*Divisor no contexto atual é para encontrar a última '/', indicando o diretório.*/
				}
					programa = StrText.substr(0, divisor);
					/*Programa(.exe) começa do primeiro char do StrTexto até onde começa o diretório*/

					/*Diretório + executável*/
					for (int i = (int)divisor, c = 0, e = 0; c == 0 && i < StrText.length(); i++)
					{
						DWORD	wFCheckFile;
						BOOL	CwFunc = GetBinaryTypeW(programa.c_str(), &wFCheckFile);/*Pega o tipo binário do arquivo*/

						/*Verifica se o arquivo é um arquivo executável*/
						if ((int)StrText[i] == 32 && CwFunc && wFCheckFile == 0 || wFCheckFile == 1 || wFCheckFile == 6)
							c = 1;
						else
							programa += StrText[i];
						if (StrText[i] == '.' && e == 0)
							e++;
						if ((int)StrText[i] == 32 && e == 1)
							c = 1;
						if (c == 1)
							divisob = i;
					}

					if (divisob > 0 && divisob < StrText.length())
					{
						/*Seperando a linha de comando e o programa (executável)*/
						linhadecomando = StrText.substr(divisob, StrText.length());

						while (linhadecomando[0] == ' ')
							linhadecomando.erase(0, 1);
					}

				}
				else
				{
					/*Exemplo: cmd [parametros]*/
					programa = tempcheck;
					if (cmdLine__ == TRUE)
						linhadecomando = StrText.substr(tempcheck.length() + 1, StrText.length());
				}
			}
			else
				programa = StrText;

			SHELLEXECUTEINFO shExInfo = { 0 };
			shExInfo.cbSize = sizeof(shExInfo);
			shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			shExInfo.hwnd = janelaAtual;
			shExInfo.lpVerb = L"open";

			shExInfo.lpFile = programa.c_str();

			if (lstrlenW(linhadecomando.c_str()) > 0)
				shExInfo.lpParameters = linhadecomando.c_str();

		shExInfo.lpDirectory = 0;
		shExInfo.nShow = SW_SHOW;

		ShellExecuteEx(&shExInfo);

		BOOL AddComboBox = FALSE;

		if (shExInfo.hProcess != 0)
		{
			if (WaitForInputIdle(shExInfo.hProcess, INFINITE) != 0)
				Sleep(200);

		wchar_t FileNameObject[260];
			K32GetModuleFileNameExW(shExInfo.hProcess, 0, FileNameObject, 260);
			programa = FileNameObject;

		if (GetFileAttributesW(programa.c_str()) != INVALID_FILE_ATTRIBUTES)
			AddComboBox = TRUE;
		if (lstrlenW(linhadecomando.c_str()) > 0)
			{
				programa += L" ";
				programa += linhadecomando;
			}
		}
		else if (shExInfo.hProcess == 0)
		{
			if (programa[programa.length() - 1] == ':')
				programa += L"\\";
			if (GetFileAttributesW(programa.c_str()) != INVALID_FILE_ATTRIBUTES)
				AddComboBox = TRUE;
		}

	if (AddComboBox == TRUE && comboBox1->Items->IndexOf(gcnew String(programa.c_str())) < 0)
			comboBox1->Items->Add(gcnew String(programa.c_str()));
	}
}

HWND HandlesVectHwnd;

private: System::Void button9_Click(System::Object^  sender, System::EventArgs^  e) {

		if (!IsWindow(HandlesVectHwnd)) /*Chechar se o formulário já está aberto*/
		{
			DWORD pid = Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value->ToString());
			string moduloname, modulepathname;

			CHAR* _moduloname, *_modulepathname;
			_moduloname		= (CHAR*)malloc(MAX_PATH);
			_modulepathname = (CHAR*)malloc(MAX_PATH);

			HANDLE hProcesso = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			GetModuleBaseNameA(hProcesso, NULL, _moduloname, MAX_PATH);
			GetModuleFileNameExA(hProcesso, NULL, _modulepathname, MAX_PATH);
			moduloname		= _moduloname;
			modulepathname	= _modulepathname;

			Form^ HandlesVect = gcnew HandlesAlone::Handles(pid, moduloname, modulepathname);
			memset(_moduloname, 0, sizeof(_moduloname));
			memset(_modulepathname, 0, sizeof(_modulepathname));
			moduloname.clear();
			modulepathname.clear();
			CloseHandle(hProcesso);

			_SAL1_1_Source_(20);
			int locX, locY;
			locX = this->Location.X + this->Size.Width / 2;		/*PontoX da metade da janela.*/
			locY = this->Location.Y + this->Size.Height / 2;	/*PontoY da metade da jaela.*/

			locX = locX - (HandlesVect->Size.Width / 2);		/*Centralizar X do form na janela*/
			locY = locY - (HandlesVect->Size.Height / 2);		/*Centralizar Y do form na janela*/

			HandlesVect->Show();
			HandlesVect->SetDesktopLocation(locX, locY);		/*Mudar a posição*/
			HandlesVectHwnd = (HWND)HandlesVect->Handle.ToInt32();
		}
		else
			BringWindowToTop(HandlesVectHwnd); /*Traz o formulário aberto para o topo*/
	}
	private: System::Void dataGridView1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {

	}
	private: System::Void button10_Click(System::Object^  sender, System::EventArgs^  e) {

		if (dataGridView1->SelectedRows[0]->Visible == true && dataGridView1->Rows[dataGridView1->SelectedRows[0]->Index]->DefaultCellStyle->BackColor == Color::Empty)
		{
			std::wstring ProcessoAtual;
			System::String ^ ProcActual = dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column1"]->Value->ToString();

			for (int i = 0; i < ProcActual->Length; i++)
				ProcessoAtual += ProcActual[i];

			if (MessageBoxW(janelaAtual, std::wstring(L"O processo \"" + ProcessoAtual + L"\" será reiniciado no seu diretório atual e com a mesma linha de comando.\nTem certeza que deseja prosseguir?").c_str(), 0, MB_ICONWARNING | MB_YESNO) == IDYES)
			{

				HANDLE hProcesso = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value->ToString()));
				if (!hProcesso)
					hProcesso = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value->ToString()));

				ULONG processCrit = 0x0;
				NtQueryInformationProcess(hProcesso, ProcessBreakOnTermination, (PVOID)&processCrit, sizeof(ProcessBreakOnTermination), NULL);
				/*Verificar se o processo é um processo crítico*/

				if (processCrit > 0l)
				{
					MessageBoxW(janelaAtual, L"Um processo crítico não pode ser finalizado.", 0, MB_ICONERROR);
					return;
				}
				else{

					hProcesso = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION, FALSE, Convert::ToInt32(dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column2"]->Value->ToString()));

					if (hProcesso == INVALID_HANDLE_VALUE || GetLastError() == ERROR_ACCESS_DENIED)
					{
						CloseHandle(hProcesso);
						MensagemDispathError(0);
					}
					else{
						STARTUPINFOA startInfo{ 0 };
						PROCESS_INFORMATION procInfo{ 0 };

						ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));
						ZeroMemory(&startInfo, sizeof(STARTUPINFOA));

						std::string programa__, linhadecomando__, diretorio__;

						/*for (int i = 0; i < textBox1->Text->Length; i++)
							programa__ += textBox1->Text[i];

						for (int i = 0; i < textBox2->Text->Length; i++)
							linhadecomando__ += textBox2->Text[i];

						for (int i = 0; i < textBox3->Text->Length; i++)
							diretorio__ += textBox3->Text[i];*/

						TerminateProcess(hProcesso, 0);
						CloseHandle(hProcesso);

						if (!CreateProcessA(programa__.c_str(), (LPSTR)linhadecomando__.c_str(), NULL, NULL, NULL, NULL, NULL, diretorio__.c_str(), &startInfo, &procInfo))
						{
							MensagemDispathError(0);
							return;
						}

						CloseHandle(procInfo.hProcess);
						CloseHandle(procInfo.hThread);
					}
				}
		}
	}
}

HWND calcWindow;

	private: System::Void calculadoraToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

		if (!IsWindow(calcWindow)) /*Verificar se a janela já existe*/
		{
			Form^ CalcForm_ = gcnew ConsoleApplication2::Calc();

			int locX, locY;
			locX = this->Location.X + this->Size.Width;
			locY = this->Location.Y;

			/*Pegar as posições para colocar o form calc ao lado do form principal*/

			CalcForm_->Show();
			CalcForm_->SetDesktopLocation(locX, locY);
			calcWindow = (HWND)CalcForm_->Handle.ToInt32();
		}
		else
			BringWindowToTop(calcWindow);
	}


	private: System::Void RestartService()
	{
		if (dataGridView2->CurrentCell->RowIndex > -1)
		{
			std::wstring ServiceName;
			for (int i = 0; i < dataGridView2->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["dataGridViewTextBoxColumn1"]->Value->ToString()->Length; i++)
				ServiceName += dataGridView2->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["dataGridViewTextBoxColumn1"]->Value->ToString()[i];

			SC_HANDLE TT_SC_MANAGER = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			SC_HANDLE TT_SC_NAME = OpenServiceW(TT_SC_MANAGER, ServiceName.c_str(), SERVICE_ALL_ACCESS);
			SERVICE_STATUS TT_SC_STATUS;
			ControlService(TT_SC_NAME, SERVICE_CONTROL_STOP, &TT_SC_STATUS);
			CloseServiceHandle(TT_SC_NAME);
			CloseServiceHandle(TT_SC_MANAGER);
		}
	}

	private: System::Void GoToProcess(System::Object^  sender, System::EventArgs^  e) /*1 - Ir para o processo*/
	{
		if (dataGridView2->CurrentCell->RowIndex > -1)
		{
			int PID_ALVO = Convert::ToInt32(dataGridView2->Rows[dataGridView2->CurrentCell->RowIndex]->Cells["Column6"]->Value->ToString());
			int IndexRowColumnPid = 0;
			for (int i = 0; i < dataGridView1->Rows->Count && IndexRowColumnPid == 0; i++)
			{
				if (Convert::ToInt32(dataGridView1->Rows[i]->Cells["Column2"]->Value->ToString()) == PID_ALVO)
					IndexRowColumnPid = i;
			}
			dataGridView1->Rows[IndexRowColumnPid]->Selected = true;
			dataGridView1->FirstDisplayedScrollingRowIndex = IndexRowColumnPid;
			tabControl1->SelectedIndex = 0;
		}
	}

	private: System::Void StartTheService(System::Object^  sender, System::EventArgs^  e) /*2 - Iniciar o serviço*/
	{
		MessageBox::Show("Iniciar o serviço");
	}

	private: System::Void RestartTheService(System::Object^  sender, System::EventArgs^  e) /*3 - Reiniciar o serviço*/
	{
		MessageBox::Show("Reiniciar o serviço");
	}

	private: System::Void StopTheService(System::Object^  sender, System::EventArgs^  e) /*4 - Interromper o serviço*/
	{
		MessageBox::Show("Interromper o serviço");
	}

	private: System::Void DeleteTheService(System::Object^  sender, System::EventArgs^  e) /*5 - Deletar o serviço*/
	{
		MessageBox::Show("Deletar o serviço");
	}

	/*Estados dos botões (true = habilitado / false = desabilitado)*/
	List<bool>^ ServiceStatesBut1 = gcnew List<bool>(); /*Ir para processo*/
	List<bool>^ ServiceStatesBut2 = gcnew List<bool>(); /*Iniciar serviço*/
	List<bool>^ ServiceStatesBut3 = gcnew List<bool>(); /*Reiniciar serviço*/
	List<bool>^ ServiceStatesBut4 = gcnew List<bool>(); /*Parar serviço*/
	List<bool>^ ServiceStatesBut5 = gcnew List<bool>(); /*Deletar serviço*/

	private: System::Void ServicesRefresh(System::Object^ sender, System::EventArgs^ e)
	{
		dataGridView2->Visible = false;
		dataGridView2->Rows->Clear();

		HGLOBAL hAllocServices;

		DWORD cbNeeded = 0, dwRet= 0, dwHandle = 0;
		SC_HANDLE hSC = OpenSCManagerW(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
		
		if (hSC == INVALID_HANDLE_VALUE)
		{
			CloseServiceHandle(hSC);
			MensagemDispathError(0);
			return;
		}

		LPENUM_SERVICE_STATUS_PROCESS ServicesEx = nullptr;

		EnumServicesStatusExW(hSC, SC_ENUM_PROCESS_INFO, SERVICE_DRIVER | SERVICE_FILE_SYSTEM_DRIVER | SERVICE_KERNEL_DRIVER |
			SERVICE_WIN32 | SERVICE_WIN32_OWN_PROCESS | SERVICE_WIN32_SHARE_PROCESS, SERVICE_STATE_ALL, (BYTE*)ServicesEx, 0,
			&cbNeeded, &dwRet, &dwHandle, NULL);

		hAllocServices = GlobalAlloc(GMEM_FIXED, cbNeeded);
		GlobalLock(hAllocServices);
		ServicesEx = (LPENUM_SERVICE_STATUS_PROCESS)hAllocServices;

		if (EnumServicesStatusExW(hSC, SC_ENUM_PROCESS_INFO, SERVICE_DRIVER | SERVICE_FILE_SYSTEM_DRIVER | SERVICE_KERNEL_DRIVER |
			SERVICE_WIN32 | SERVICE_WIN32_OWN_PROCESS | SERVICE_WIN32_SHARE_PROCESS, SERVICE_STATE_ALL, (BYTE*)ServicesEx, cbNeeded,
			&cbNeeded, &dwRet, &dwHandle, NULL) == FALSE)
		{
			GlobalUnlock(hAllocServices);
			GlobalFree(hAllocServices);
			MensagemDispathError(0);
			return;
		}

		for (DWORD i = 0; i < dwRet; i++)
		{
			std::wstring nameService = ServicesEx[i].lpServiceName;
			std::wstring displayService = ServicesEx[i].lpDisplayName;
			std::wstring typeService = L"Desconhecido.";
			std::wstring stateService = L"Desconhecido.";

			DWORD assocPID_ = ServicesEx[i].ServiceStatusProcess.dwProcessId;

			switch (ServicesEx[i].ServiceStatusProcess.dwServiceType)
			{
			case SERVICE_FILE_SYSTEM_DRIVER:
				typeService = L"FS Driver";
				break;
			case SERVICE_KERNEL_DRIVER:
				typeService = L"Kernel";
				break;
			case SERVICE_WIN32_OWN_PROCESS:
				typeService = L"Own process";
				break;
			case SERVICE_WIN32_SHARE_PROCESS:
				typeService = L"Share process";
				break;
			case SERVICE_INTERACTIVE_PROCESS:
				typeService = L"Interactive";
				break;
			}

			switch (ServicesEx[i].ServiceStatusProcess.dwCurrentState)
			{
			case SERVICE_CONTINUE_PENDING:
				stateService = L"Continuando";
				ServiceStatesBut2->Add(false);
				ServiceStatesBut3->Add(false);
				ServiceStatesBut4->Add(true);
				break;
			case SERVICE_PAUSE_PENDING:
				stateService = L"Pausando";
				ServiceStatesBut2->Add(false);
				ServiceStatesBut3->Add(false);
				ServiceStatesBut4->Add(false);
				break;
			case SERVICE_PAUSED:
				stateService = L"Pausado";
				ServiceStatesBut2->Add(true);
				ServiceStatesBut3->Add(true);
				ServiceStatesBut4->Add(false);
				break;
			case SERVICE_RUNNING:
				stateService = L"Execução";
				ServiceStatesBut2->Add(false);
				ServiceStatesBut3->Add(true);
				ServiceStatesBut4->Add(true);
				break;
			case SERVICE_START_PENDING:
				stateService = L"Iniciando";
				ServiceStatesBut2->Add(false);
				ServiceStatesBut3->Add(false);
				ServiceStatesBut4->Add(true);
				break;
			case SERVICE_STOP_PENDING:
				stateService = L"Parando";
				ServiceStatesBut2->Add(true);
				ServiceStatesBut3->Add(false);
				ServiceStatesBut4->Add(false);
				break;
			case SERVICE_STOPPED:
				stateService = L"Parado";
				ServiceStatesBut2->Add(true);
				ServiceStatesBut3->Add(false);
				ServiceStatesBut4->Add(false);
				break;
			}

			String^ _nameService = gcnew String(nameService.c_str());
			String^ _displayName = gcnew String(displayService.c_str());
			String^ _typeService = gcnew String(typeService.c_str());
			String^ _stateService = gcnew String(stateService.c_str());
			String^ _servicePid;
			if (assocPID_ == 0)
			{
				_servicePid = gcnew String("-");
				ServiceStatesBut1->Add(false);
			}
			else
			{
				_servicePid = gcnew String(assocPID_.ToString());
				ServiceStatesBut1->Add(true);
			}

			ServiceStatesBut5->Add(true);
			dataGridView2->Rows->Add(_nameService, _displayName, _typeService, _stateService, _servicePid);
		}
		dataGridView2->Visible = true;
		GlobalUnlock(hAllocServices);
		GlobalFree(hAllocServices);
	}

	private: System::Void tabControl1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		//Column6
		if (tabControl1->SelectedIndex == 1)/*Serviços*/
		{
			ServicesRefresh(nullptr, nullptr); /*nullptr = pointer nulo / pontero vazio / espaço da memória em branco*/
		}
	}
	private: System::Void progressBar1_Click(System::Object^  sender, System::EventArgs^  e) {
	}

	private: System::Void dataGridView1_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {

	}
	private: System::Void leitorDeTextoToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		char filename[MAX_PATH];
		WCHAR currentdir[MAX_PATH];

		GetCurrentDirectoryW(MAX_PATH, currentdir);

		OPENFILENAMEA ofn;
		ZeroMemory(filename, sizeof(filename));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = janelaAtual;
		ofn.lpstrFilter = "Arquivos de texto\*.txt\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Selecione o arquivo";
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameA(&ofn))
		{
			std::string fileWant = filename;
			Form^ TT_FORM_FILE = gcnew ConsoleApplication2::FileReader(fileWant, this->FindForm());			
			fileWant.clear();
		
		}

		SetCurrentDirectoryW(currentdir);
		ZeroMemory(&ofn, sizeof(&ofn));
		ZeroMemory(currentdir, sizeof(currentdir));
		ZeroMemory(filename, sizeof(filename));

	}
	private: System::Void dataGridView2_SelectionChanged(System::Object^  sender, System::EventArgs^  e) {
		if (dataGridView2->CurrentCell->RowIndex > -1)
		{
			TT_B1->Enabled	= ServiceStatesBut1[dataGridView2->CurrentCell->RowIndex];
			TT_B2->Enabled	= ServiceStatesBut2[dataGridView2->CurrentCell->RowIndex];
			TT_B3->Enabled	= ServiceStatesBut3[dataGridView2->CurrentCell->RowIndex];
			TT_B4->Enabled	= ServiceStatesBut4[dataGridView2->CurrentCell->RowIndex];
			TT_B5->Enabled	= ServiceStatesBut5[dataGridView2->CurrentCell->RowIndex];
		}

	}

};
}

/*
LPCVOID hehe = &(((_RTL_USER_PROCESS_PARAMETERS*)EnderecoParametros)->CurrentDirectory.DosPath);

0x000000000001000

char charletgo[120];
sprintf(charletgo, "0x%p", hehe);
MessageBoxA(0, charletgo, 0, 0);
*/

DWORD dwGetModuleBaseAddress(DWORD dwProcessIdentifier, TCHAR *lpszModuleName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessIdentifier);
	DWORD dwModuleBaseAddress = 0;
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 ModuleEntry32 = { 0 };
		ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &ModuleEntry32))
		{
			do
			{
				if (wcscmp(ModuleEntry32.szModule, lpszModuleName) == 0)
				{
					dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnapshot, &ModuleEntry32));
		}
		CloseHandle(hSnapshot);
	}
	return dwModuleBaseAddress;
}

bool SameFile(HANDLE h1, HANDLE h2) {
	BY_HANDLE_FILE_INFORMATION bhfi1 = { 0 };
	BY_HANDLE_FILE_INFORMATION bhfi2 = { 0 };
	if (GetFileInformationByHandle(h1, &bhfi1) && GetFileInformationByHandle(h2, &bhfi2)) {
		return ((bhfi1.nFileIndexHigh == bhfi2.nFileIndexHigh) &&
			(bhfi1.nFileIndexLow == bhfi2.nFileIndexLow) &&
			(bhfi1.dwVolumeSerialNumber == bhfi2.dwVolumeSerialNumber));
	}
	return false;
}

int CALLBACK _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew ConsoleApplication2::MyForm());
	TerminateProcess(GetCurrentProcess(), 0);
	
	return false;
}