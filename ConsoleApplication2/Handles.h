#include "HandlesStructs.h"
#pragma comment(lib, "Mpr.lib")

#pragma once

namespace HandlesAlone {
	
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;

	/// <summary>
	/// Summary for Handles
	/// </summary>
	public ref class Handles : public System::Windows::Forms::Form
	{
	public:
		
	DWORD processo_handle_alvo = 0;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage4;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::DataGridView^  dataGridView2;
	private: System::Windows::Forms::Button^  button9;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn3;








	private: System::Windows::Forms::TabPage^  tabPage3;		 

System::Void HandlesRefresh()
{
	SendMessageW((HWND)dataGridView1->Handle.ToInt32(), WM_SETREDRAW, FALSE, 0);
	dataGridView1->Visible = false; /*Iniciar como invível para melhor desempenho*/

	DWORD idALPCPort = 0;
	DWORD idArquivos = 0;
	DWORD idDesktop = 0;
	DWORD idDiretorios = 0;
	DWORD idEventos = 0;
	DWORD idIoCompletion = 0;
	DWORD idKeyedEvents = 0;
	DWORD idMutex = 0;
	DWORD idProcessos = 0;
	DWORD idRegKeys = 0;
	DWORD idSemaforos = 0;
	DWORD idSessoes = 0;
	DWORD idThreads = 0;
	DWORD idTimer = 0;
	DWORD idToken = 0;
	DWORD idTpWorkerFactory = 0;
	DWORD idWindowStation = 0;

	DWORD idHandlesContagem = 0;
	DWORD idVisibleHandles = 0;

	DWORD pid = processo_handle_alvo;

	/*Funções não-documentadas... Precisa importar a função manualmente, pegando o endereço na DLL e definindo estruturas*/
	NTSTATUS status;
	PSYSTEM_HANDLE_INFORMATION handleInfo;
	ULONG AlocarTamanho = 1;

	HANDLE ProcessHandlesDump = OpenProcess(PROCESS_DUP_HANDLE | PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	GetProcessHandleCount(ProcessHandlesDump, &idHandlesContagem); /*Pegar a quantidade handles em um processo*/
	handleInfo = (PSYSTEM_HANDLE_INFORMATION)malloc(AlocarTamanho);
	/*malloc para alocar a estrutura da função em um espaço necessário da memória*/

	while ((status = NtQuerySystemInformation(SystemHandleInformation, handleInfo, AlocarTamanho, NULL)) == STATUS_INFO_LENGTH_MISMATCH)
	{
		AlocarTamanho *= 2;
		handleInfo = (PSYSTEM_HANDLE_INFORMATION)realloc(handleInfo, AlocarTamanho);
		/*STATUS_INFO_LENGTH_MISMATCH -> INDICA QUE A FUNÇÃO NÃO CONSEGUIU SE ALOCAR NO ESPAÇO INDICADO por isso usa-se o realloc.*/
		/*Fica num loop realocando o tamanho em dobro até conseguir tamanho necessário para a estrutura*/
	}

	int SimpleCont = 0;
	std::string _hHandle, _hNome, _hTipo;
	String^ __Tipo, ^__Nome, ^__Handle;
	char Tipo__[1024], Nome__[1024], Handle__[1024];

	for (int i = 0; i < handleInfo->HandleCount; i++)
	{
		SYSTEM_HANDLE handle = handleInfo->Handles[i];
		HANDLE HandleDuplicada__ = NULL;
		POBJECT_TYPE_INFORMATION NomeHandleTipo;
		PVOID InfoObjeto;
		UNICODE_STRING NomeHandleObjeto;
		ULONG valorRetorno;

		if (handle.ProcessId != pid)
			continue; /*Simula como se estivesse no final do loop e vai para o próximo incremento (i++)*/

		NT_SUCCESS(NtDuplicateObject(ProcessHandlesDump, (HANDLE)handle.Handle, GetCurrentProcess(), &HandleDuplicada__, 0, 0, 0));

		NomeHandleTipo = (POBJECT_TYPE_INFORMATION)malloc(0x1000);
		NT_SUCCESS(NtQueryObject(HandleDuplicada__, ObjectTypeInformation, NomeHandleTipo, 0x1000, NULL));

		if (handle.GrantedAccess == 0x0012019f)
		{
			std::free(NomeHandleTipo);
			CloseHandle(HandleDuplicada__);
			continue;
		}

		InfoObjeto = malloc(0x1000);
		if (!NT_SUCCESS(NtQueryObject(HandleDuplicada__, ObjectNameInformation, InfoObjeto, 0x1000, &valorRetorno)))
		{
			InfoObjeto = realloc(InfoObjeto, valorRetorno);
			if (!NT_SUCCESS(NtQueryObject(HandleDuplicada__, ObjectNameInformation, InfoObjeto, valorRetorno, NULL)))
			{
				std::free(NomeHandleTipo);
				std::free(InfoObjeto);
				CloseHandle(HandleDuplicada__);
				continue;
			}
		}
		NomeHandleObjeto = *(PUNICODE_STRING)InfoObjeto;

		SimpleCont++;

		memset(Tipo__, 0, sizeof(Tipo__)); /*Deixar o array zerado*/
		memset(Nome__, 0, sizeof(Nome__));
		memset(Handle__, 0, sizeof(Handle__));

		sprintf(Tipo__, "%.*S", NomeHandleTipo->TypeName.Length / 2, NomeHandleTipo->TypeName.Buffer);	/*TIPO*/
		sprintf(Nome__, "%.*S", NomeHandleObjeto.Length / 2, NomeHandleObjeto.Buffer);						/*NOME*/
		sprintf(Handle__, "0x%p", handleInfo->Handles[i].Handle); 								/*HANDLE*/

		_hHandle = Handle__;
		_hNome = Nome__;
		_hTipo = Tipo__;

		while (_hHandle[2] == '0')
			_hHandle.erase(2, 1); /*Remover todos '0' do segundo caractere*/

		std::transform(_hHandle.begin(), _hHandle.end(), _hHandle.begin(), tolower); /*Deixar todos os caracteres minúsculos.*/

		if (strlen(_hNome.c_str()) <= 1)
			_hNome = "Desconhecido.";

		if (_hTipo == "Thread")
		{
			HANDLE __DumpH = INVALID_HANDLE_VALUE; /*Iniciada como nula*/
			DuplicateHandle(ProcessHandlesDump, (HANDLE)handle.Handle, GetCurrentProcess(), &__DumpH, 0, false, DUPLICATE_SAME_ACCESS);
			/*Duplicar a handle no nosso processo para obter informações*/

			DWORD idOfThreadGet = GetThreadId(__DumpH);

			wchar_t ProcessThreadName[MAX_PATH];
			GetModuleBaseNameW(ProcessHandlesDump, NULL, ProcessThreadName, MAX_PATH);

			std::wstringstream broadcastXD;
			broadcastXD << ProcessThreadName << L" (" << GetProcessId(ProcessHandlesDump) << L"): " << idOfThreadGet;
			std::wstring RetornoFinal;
			RetornoFinal = broadcastXD.str();
			broadcastXD.clear();
			_hNome = std::string(RetornoFinal.begin(), RetornoFinal.end());
			RetornoFinal.clear();

			CloseHandle(__DumpH);
			idThreads++;
		}

		else if (_hTipo == "Process")
		{
			HANDLE __DumpH = INVALID_HANDLE_VALUE;

			DuplicateHandle(ProcessHandlesDump, (HANDLE)handle.Handle, GetCurrentProcess(), &__DumpH, 0, false, DUPLICATE_SAME_ACCESS);

			CHAR NameBlock[256];
			K32GetProcessImageFileNameA(__DumpH, NameBlock, 256); /*Pegar o local do arquivo com executável*/
			PathStripPathA(NameBlock);				/*Remove todos os caracteres, exceto o nome do executável*/
			DWORD idOfProcesso = GetProcessId(__DumpH);
			std::stringstream broadcastXD;
			broadcastXD << NameBlock << " (" << std::to_string(idOfProcesso) << ")";
			broadcastXD.flush();

			_hNome.clear();
			_hNome = broadcastXD.str();

			broadcastXD.clear();

			CloseHandle(__DumpH);
			idProcessos++;
		}

		else if (_hTipo == "File")
		{
			CHAR TempName__[1024];
			HANDLE __DumpH = INVALID_HANDLE_VALUE;

			DuplicateHandle(ProcessHandlesDump, (HANDLE)handle.Handle, GetCurrentProcess(), &__DumpH, 0, false, DUPLICATE_SAME_ACCESS);

			GetFinalPathNameByHandleA(__DumpH, TempName__, 1024, VOLUME_NAME_DOS);

			CloseHandle(__DumpH);

			_hNome = TempName__;
			_hNome.erase(0, 4);
			idArquivos++;
		}

		else if (_hTipo == "Key")
		{
			idRegKeys++;
			HANDLE __DumpH = INVALID_HANDLE_VALUE;

			const char* NomeRegEdits[]{"HKEY_USERS", "HKEY_LOCAL_MACHINE", "HKEY_CLASSES_ROOT", "HKEY_CURRENT_CONFIG"};
			const char* NomeVirtual[]{"\\REGISTRY\\USER", "\\REGISTRY\\MACHINE",
				"\\REGISTRY\\MACHINE\\SOFTWARE\\Classes", "\\REGISTRY\\MACHINE\\SYSTEM\\CURRENTCONTROLSET\\HARDWARE PROFILES\\CURRENT"};

			for (int i = 0; i < 4; i++)
			{
				if (strcmp(_hNome.substr(0, strlen(NomeVirtual[i])).c_str(), NomeVirtual[i]) == 0 &&
					strcmp(_hNome.substr(0, strlen(NomeVirtual[3])).c_str(), NomeVirtual[3]) == 0)
				{
					_hNome.erase(0, strlen(NomeVirtual[3]));
					if (_hNome[0] == '\\')
						_hNome = (std::string)NomeRegEdits[3] + _hNome;
					else
						_hNome = (std::string)NomeRegEdits[3] + "\\" + _hNome;

					break;
				}
				else{
					if (strcmp(_hNome.substr(0, strlen(NomeVirtual[i])).c_str(), NomeVirtual[i]) == 0)
					{
						_hNome.erase(0, strlen(NomeVirtual[i]));
						if (_hNome[0] == '\\')
							_hNome = (std::string)NomeRegEdits[i] + _hNome;
						else
							_hNome = (std::string)NomeRegEdits[i] + "\\" + _hNome;

						break;
					}
				}
			}

			if (_hNome[_hNome.length() - 1] == '\\')
				_hNome.pop_back();	/*Remove o último caractere*/

			CloseHandle(__DumpH);
		}
		else{
			__Nome = gcnew String(Nome__);

			if (_hTipo == "ALPC Port")
				idALPCPort++;
			else if (_hTipo == "Desktop")
				idDesktop++;
			else if (_hTipo == "Directory")
				idDiretorios++;
			else if (_hTipo == "Event")
				idEventos++;
			else if (_hTipo == "Mutant")
				idMutex++;
			else if (_hTipo == "IoCompletion")
				idIoCompletion++;
			else if (_hTipo == "KeyedEvent")
				idKeyedEvents++;
			else if (_hTipo == "Timer")
				idTimer++;
			else if (_hTipo == "TpWorkerFactory")
				idTpWorkerFactory++;
			else if (_hTipo == "Semaphore")
				idSemaforos++;
			else if (_hTipo == "Token")
				idToken++;
			else if (_hTipo == "Section")
				idSessoes++;
			else if (_hTipo == "WindowStation")
				idWindowStation++;
		}

		__Tipo = gcnew String(_hTipo.c_str());
		__Handle = gcnew String(_hHandle.c_str());

		if (_hNome.length() > 1)
			__Nome = gcnew String(_hNome.c_str());
		else
			__Nome = gcnew String(Nome__);


		if (__Nome != "Desconhecido."){
			idVisibleHandles++;
		}

		//dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->DefaultCellStyle->BackColor = Color::Gray;
		
		dataGridView1->Rows->Add(__Tipo, __Nome, __Handle);

		if (handleInfo->Handles[i].Flags == HANDLE_FLAG_INHERIT)
			dataGridView1->Rows[SimpleCont - 1]->DefaultCellStyle->BackColor = Color::Gray;
		else if (handleInfo->Handles[i].Flags == HANDLE_FLAG_PROTECT_FROM_CLOSE)
			dataGridView1->Rows[SimpleCont - 1]->DefaultCellStyle->BackColor = Color::LightSkyBlue;

		std::free(NomeHandleTipo);
		std::free(InfoObjeto);
		CloseHandle(HandleDuplicada__);
	}

	std::free(handleInfo);
	CloseHandle(ProcessHandlesDump);

	/*label2->Text = "Contagem handles: "			+ idHandlesContagem.ToString();
	label3->Text = "Handles nomeadas: "				+ idVisibleHandles.ToString();

	bALPCPort->Text = "ALPC Port: "					+ idALPCPort.ToString();
	bArquivos->Text = "Arquivos: "					+ idArquivos.ToString();
	bDesktop->Text = "Desktop: "					+ idDesktop.ToString();
	bDiretorios->Text = "Diretórios: "				+ idDiretorios.ToString();
	bEventos->Text = "Eventos: "					+ idEventos.ToString();
	bIoCompletion->Text = "IoCompletion: "			+ idIoCompletion.ToString();
	bKeyedEvents->Text = "KeyedEvents: "			+ idKeyedEvents.ToString();
	bMutex->Text = "Mutex: "						+ idMutex.ToString();
	bProcessos->Text = "Processos: "				+ idProcessos.ToString();
	bRegKeys->Text = "RegKeys: "					+ idRegKeys.ToString();
	bSemaforos->Text = "Semáforos: "				+ idSemaforos.ToString();
	bSessoes->Text = "Sessões: "					+ idSessoes.ToString();
	bThreads->Text = "Threads: "					+ idThreads.ToString();
	bTimer->Text = "Timer: "						+ idTimer.ToString();
	bToken->Text = "Token: "						+ idToken.ToString();
	bTpWorkerFactory->Text = "TpWorkerFactory: "	+ idTpWorkerFactory.ToString();
	bWindowStation->Text = "WindowStation: "		+ idWindowStation.ToString();*/
	/*PARA USO FUTURO ~ PLEASE NOT ERASE THIS!!!*/

	SendMessageW((HWND)dataGridView1->Handle.ToInt32(), WM_SETREDRAW, TRUE, 0);
	dataGridView1->Visible = true;
	dataGridView1->Refresh();

}


const wchar_t* GetThreadModuleStartAddress(int threadId)
{
		HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, false, threadId);
		DWORD pidIs = GetProcessIdOfThread(hThread);
		HGLOBAL buf = GlobalAlloc(GMEM_FIXED, sizeof(INT_PTR));
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pidIs);

		NTSTATUS result = NtQueryInformationThread(hThread, ThreadQuerySetWin32StartAddress, buf, GlobalSize(buf), nullptr);
		if (!NT_SUCCESS(result))
			return 0;
		CloseHandle(hThread);
		GlobalLock(buf);

		std::vector<BYTE*>MeuVetor;
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(MODULEENTRY32);

		wstring retornois = L"Nulo";
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pidIs);
		Module32First(hSnapshot, &modEntry);
		do{
			if (modEntry.th32ProcessID == pidIs)
				MeuVetor.push_back(modEntry.modBaseAddr);
		} while (Module32Next(hSnapshot, &modEntry));
		CloseHandle(hSnapshot);

		std::sort(MeuVetor.begin(), MeuVetor.end());

		BYTE* eiijota = *((BYTE**)buf);
		int i;
		wchar_t modnameis[260];

		for (i = 0; i < MeuVetor.size(); i++)
		{
			if (i == MeuVetor.size() - 1)
			{
				GetModuleBaseNameW(hProcess, (HMODULE)MeuVetor[i], modnameis, 260);
				retornois = modnameis;
				break;
			}
			else if (eiijota >= MeuVetor[i] && eiijota < MeuVetor[i + 1])
			{
				GetModuleBaseNameW(hProcess, (HMODULE)MeuVetor[i], modnameis, 260);
				retornois = modnameis;
				break;
			}
		}

		wchar_t *ThreadAddressFinal = (wchar_t*)malloc(260);
		wsprintfW(ThreadAddressFinal, L"%s+0x%x", retornois.c_str(), *((BYTE**)buf) - MeuVetor[i]);		

		MeuVetor.clear();
		GlobalUnlock(buf);
		GlobalFree(buf);

		CloseHandle(hThread);
		CloseHandle(hProcess);

		return ThreadAddressFinal;
}

System::Void ThreadsRefresh()
{
	DWORD pid = processo_handle_alvo;
	std::vector<DWORD>VectorListAll;

	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	Thread32First(hSnapshot, &threadEntry);
	do{
		if (threadEntry.th32OwnerProcessID == pid)
			VectorListAll.push_back(threadEntry.th32ThreadID);
	} while (Thread32Next(hSnapshot, &threadEntry));

	std::sort(VectorListAll.begin(), VectorListAll.end());

	CloseHandle(hSnapshot);

	int Priority;
	HANDLE hThread__;
	String^ PriorityName, ^StartAddressName, ^ThreadIDName;
	dataGridView2->Visible = false;
	for (int i = 0; i < VectorListAll.size(); i++)
	{
		hThread__ = OpenThread(THREAD_QUERY_LIMITED_INFORMATION, FALSE, VectorListAll[i]);
		Priority = 0;
		Priority = GetThreadPriority(hThread__);
		CloseHandle(hThread__);

		switch (Priority)
		{
			case THREAD_PRIORITY_ABOVE_NORMAL:
				PriorityName = gcnew String("Acima do normal");
			break;
			case THREAD_PRIORITY_BELOW_NORMAL:
				PriorityName = gcnew String("Abaixo do normal");
				break;
			case THREAD_PRIORITY_HIGHEST:
				PriorityName = gcnew String("Alta");
				break;
			case THREAD_PRIORITY_IDLE:
				PriorityName = gcnew String("Inativa");
				break;
			case THREAD_PRIORITY_LOWEST:
				PriorityName = gcnew String("Baixa");
				break;
			case THREAD_PRIORITY_NORMAL:
				PriorityName = gcnew String("Normal");
				break;
			case THREAD_PRIORITY_TIME_CRITICAL:
				PriorityName = gcnew String("Crítico");
				break;

			default:
				PriorityName = gcnew String(Priority.ToString());
				break;
		}

		StartAddressName = gcnew String(GetThreadModuleStartAddress(VectorListAll[i]));
		ThreadIDName = gcnew String(VectorListAll[i].ToString());
		dataGridView2->Rows->Add(ThreadIDName, StartAddressName, PriorityName);
	}

	VectorListAll.clear();
	dataGridView2->Visible = true;
}

std::wstring findpidwithname(DWORD processid)
{
	if (processid == 0)
		return L"Processo não identificado";

	WTS_PROCESS_INFOW* WTSSTRUCT; /*Estrutura para receber os processos*/
	DWORD contagem; /*Quantidade de processos*/
	/*WTS_CURRENT_SERVER_HANDLE = COMPUTADOR ATUAL*/
	WTSEnumerateProcessesW(WTS_CURRENT_SERVER_HANDLE, 0, 1, &WTSSTRUCT, &contagem);
	for (int i = 0; i < contagem; i++)
	{
		if (WTSSTRUCT[i].ProcessId == processid)
		{
			const wchar_t* ProcessoRetorno = WTSSTRUCT[i].pProcessName;
			WTSFreeMemory(WTSSTRUCT);
			return ProcessoRetorno;
		}
	}
	WTSFreeMemory(WTSSTRUCT); /*Esvaziar a estrutura e os processos da memória*/
	return L"Processo não identificado";
}

System::Void PropertiesViewer()
{
	PROCESS_BASIC_INFORMATION pbi;
	DWORD pidSelecionado = processo_handle_alvo;
	HANDLE hProcMy = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pidSelecionado);

	/*PROCESS_QUERY_INFORMATION PARA USAR A FUNÇÃO NtQueryInformationProcess->EndereçoPeb para pegar imagem e linha de comando.*/
	/*PROCESS_VM_READ para que depois de pegarmos o endereço PEB, termos permissão de ler o que está escrito nessa memória.*/

	char ProcessParent[260];
	ULONG_PTR PidParent[6];
	ULONG ulSize = 0;

	NtQueryInformationProcess(hProcMy, (PROCESSINFOCLASS)0, &PidParent, sizeof(PidParent), &ulSize);

	if ((pidSelecionado == 0) || (pidSelecionado == 4))
		PidParent[5] = 0;

	wstring tempwstr = findpidwithname(PidParent[5]);
	string tempstr_(tempwstr.begin(), tempwstr.end());

	strcpy(ProcessParent, tempstr_.c_str());

	tempstr_.clear();
	tempwstr.clear();

	//textBox5->Text = gcnew String(std::string((std::string)ProcessParent + " (" + to_string(PidParent[5]) + ")").c_str());

	VirtualFree(ProcessParent, sizeof(ProcessParent), MEM_RELEASE);

	NTSTATUS NtQueryCheck = NtQueryInformationProcess(hProcMy, ProcessBasicInformation, &pbi, sizeof(pbi), 0);

	if (!NT_SUCCESS(NtQueryCheck) || !hProcMy)
	{
		WCHAR ErrorDispath[1024];
		DWORD ErrorId = GetLastError();
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ErrorId, 0, ErrorDispath, sizeof(ErrorDispath), NULL);
		//textBox1->Text = gcnew String(ErrorDispath);
		//textBox2->Text = gcnew String(ErrorDispath);
		//textBox3->Text = gcnew String(ErrorDispath);
		//textBox4->Text = gcnew String(ErrorDispath);
		memset(&ErrorDispath, 0, sizeof(ErrorDispath));
		return;
	}
	::FILETIME		fpCreationTime, fpExitTime, fpKernelTime, fpUserTime;
	::FILETIME		cpCreationTime, cpExitTime, cpKernelTime, cpUserTime;
	::SYSTEMTIME	ntCreationTime, ntExitTime, ntKernelTime, ntUserTime, ntLocalTime;

	GetLocalTime(&ntLocalTime);
	char* printtime = (char*)malloc(260);
	char* printdata = (char*)malloc(260);

	string timelepse_;

	GetProcessTimes(hProcMy, &fpCreationTime, &fpExitTime, &fpKernelTime, &fpUserTime);
	
	FileTimeToLocalFileTime(&fpCreationTime, &cpCreationTime);
	FileTimeToLocalFileTime(&fpExitTime, &cpExitTime);
	FileTimeToLocalFileTime(&fpKernelTime, &cpKernelTime);
	FileTimeToLocalFileTime(&fpUserTime, &cpUserTime);

	FileTimeToSystemTime(&cpCreationTime, &ntCreationTime);
	FileTimeToSystemTime(&cpExitTime, &ntExitTime);
	FileTimeToSystemTime(&cpKernelTime, &ntKernelTime);
	FileTimeToSystemTime(&cpUserTime, &ntUserTime);

	//sprintf(printtime, "%hu:%hu:%hu", ntCreationTime.wHour, ntCreationTime.wMinute, ntCreationTime.wSecond);
	GetTimeFormatA(LOCALE_USER_DEFAULT, NULL, &ntCreationTime, "HH:mm:ss", printtime, MAX_PATH);
	GetDateFormatA(LOCALE_USER_DEFAULT, NULL, &ntCreationTime, "dd/MM/yyyy", printdata, MAX_PATH);

	//textBox4->Text = gcnew String(string((string)printtime + " " + printdata).c_str());

	//char TextoConvertido1[256], TextoConvertido2[256];
	//WideCharToMultiByte(CP_UTF7, 0, pbi.PebBaseAddress->ProcessParameters->ImagePathName.Buffer, -1, TextoConvertido1, pbi.PebBaseAddress->ProcessParameters->ImagePathName.Length, 0, 0);
	//WideCharToMultiByte(CP_UTF7, 0, pbi.PebBaseAddress->ProcessParameters->CommandLine.Buffer, -1, TextoConvertido2, pbi.PebBaseAddress->ProcessParameters->CommandLine.Length, 0, 0);

	DWORD EnderecoParametros;
	UNICODE_STRING LinhaDeComando__, ImageDoArquivo__, DiretorioLocal__;
	WCHAR *LinharDeComandoStr__, *ImagemDoArquivoStr__, *DiretorioLocalStr__;
	CHAR charLinhaDeComandoStr__[1024], charImagemDoArquivoStr__[1024], charDiretorioLocalStr__[1024];

	/*0x24*/
	ReadProcessMemory(hProcMy, &(((_PEB*)pbi.PebBaseAddress)->ProcessParameters), &EnderecoParametros, sizeof(PVOID), NULL);

	ReadProcessMemory(hProcMy, &(((_RTL_USER_PROCESS_PARAMETERS*)EnderecoParametros)->CommandLine), &LinhaDeComando__, sizeof(LinhaDeComando__), NULL);
	ReadProcessMemory(hProcMy, &(((_RTL_USER_PROCESS_PARAMETERS*)EnderecoParametros)->ImagePathName), &ImageDoArquivo__, sizeof(ImageDoArquivo__), NULL);
	ReadProcessMemory(hProcMy, &(((_RTL_USER_PROCESS_PARAMETERS*)EnderecoParametros)->CurrentDirectory.DosPath), &DiretorioLocal__, sizeof(DiretorioLocal__), NULL);

	ImagemDoArquivoStr__ = (WCHAR *)malloc(ImageDoArquivo__.Length);
	LinharDeComandoStr__ = (WCHAR *)malloc(LinhaDeComando__.Length);
	DiretorioLocalStr__ = (WCHAR *)malloc(DiretorioLocal__.Length);

	ReadProcessMemory(hProcMy, LinhaDeComando__.Buffer, LinharDeComandoStr__, LinhaDeComando__.Length, NULL);
	ReadProcessMemory(hProcMy, ImageDoArquivo__.Buffer, ImagemDoArquivoStr__, ImageDoArquivo__.Length, NULL);
	ReadProcessMemory(hProcMy, DiretorioLocal__.Buffer, DiretorioLocalStr__, DiretorioLocal__.Length, NULL);

	sprintf(charLinhaDeComandoStr__, "%.*S", LinhaDeComando__.Length / 2, LinharDeComandoStr__);
	sprintf(charImagemDoArquivoStr__, "%.*S", ImageDoArquivo__.Length / 2, ImagemDoArquivoStr__);
	sprintf(charDiretorioLocalStr__, "%.*S", DiretorioLocal__.Length / 2, DiretorioLocalStr__);

	free(ImagemDoArquivoStr__);
	free(LinharDeComandoStr__);
	free(DiretorioLocalStr__);

	//textBox1->Text = gcnew String(charImagemDoArquivoStr__);
	//textBox2->Text = gcnew String(charLinhaDeComandoStr__);
	//textBox3->Text = gcnew String(charDiretorioLocalStr__);

	CloseHandle(hProcMy);
}
	public:	Handles(int ProcessIdHandles, std::string ModuloName, std::string ModuloLocalPath)
		{
			InitializeComponent();

			SetWindowTextA((HWND)this->Handle.ToInt32(), std::string("Exibindo informações de " + ModuloName + " (" + std::to_string(ProcessIdHandles) + ")").c_str());
			
			DWORD arquivoType[2];
			if (GetBinaryTypeA(ModuloLocalPath.c_str(), &arquivoType[0]) == TRUE)
				this->Icon = System::Drawing::Icon::ExtractAssociatedIcon(gcnew String(ModuloLocalPath.c_str()));
			processo_handle_alvo = ProcessIdHandles;
			
			HandlesRefresh();
			ThreadsRefresh();
			
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Handles()
		{
			if (components)
			{
				delete components;
			}
		}


private: System::ComponentModel::IContainer^  components;
protected: private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->dataGridView2 = (gcnew System::Windows::Forms::DataGridView());
			this->dataGridViewTextBoxColumn1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->button9 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView2))->BeginInit();
			this->SuspendLayout();
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->AllowUserToResizeRows = false;
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
			this->dataGridView1->BackgroundColor = System::Drawing::SystemColors::Window;
			this->dataGridView1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->dataGridView1->CellBorderStyle = System::Windows::Forms::DataGridViewCellBorderStyle::SingleHorizontal;
			this->dataGridView1->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::None;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {
				this->Column1,
					this->Column2, this->Column3
			});
			this->dataGridView1->Cursor = System::Windows::Forms::Cursors::Default;
			this->dataGridView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dataGridView1->GridColor = System::Drawing::SystemColors::Window;
			this->dataGridView1->Location = System::Drawing::Point(3, 3);
			this->dataGridView1->MultiSelect = false;
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::None;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->ShowCellErrors = false;
			this->dataGridView1->ShowCellToolTips = false;
			this->dataGridView1->ShowEditingIcon = false;
			this->dataGridView1->ShowRowErrors = false;
			this->dataGridView1->Size = System::Drawing::Size(768, 402);
			this->dataGridView1->TabIndex = 1;
			// 
			// Column1
			// 
			this->Column1->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
			this->Column1->Frozen = true;
			this->Column1->HeaderText = L"Tipo";
			this->Column1->Name = L"Column1";
			this->Column1->ReadOnly = true;
			this->Column1->Width = 53;
			// 
			// Column2
			// 
			this->Column2->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->Column2->HeaderText = L"Nome";
			this->Column2->MinimumWidth = 370;
			this->Column2->Name = L"Column2";
			this->Column2->ReadOnly = true;
			// 
			// Column3
			// 
			this->Column3->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
			this->Column3->FillWeight = 50;
			this->Column3->HeaderText = L"Identificador";
			this->Column3->Name = L"Column3";
			this->Column3->ReadOnly = true;
			this->Column3->Width = 90;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage4);
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Location = System::Drawing::Point(12, 12);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(782, 434);
			this->tabControl1->TabIndex = 4;
			// 
			// tabPage4
			// 
			this->tabPage4->Location = System::Drawing::Point(4, 22);
			this->tabPage4->Name = L"tabPage4";
			this->tabPage4->Padding = System::Windows::Forms::Padding(3);
			this->tabPage4->Size = System::Drawing::Size(774, 408);
			this->tabPage4->TabIndex = 3;
			this->tabPage4->Text = L"Geral";
			this->tabPage4->UseVisualStyleBackColor = true;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->dataGridView1);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(774, 408);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Handles";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->dataGridView2);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(774, 408);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Threads";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// dataGridView2
			// 
			this->dataGridView2->AllowUserToAddRows = false;
			this->dataGridView2->AllowUserToDeleteRows = false;
			this->dataGridView2->AllowUserToResizeRows = false;
			this->dataGridView2->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
			this->dataGridView2->BackgroundColor = System::Drawing::SystemColors::Window;
			this->dataGridView2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->dataGridView2->CellBorderStyle = System::Windows::Forms::DataGridViewCellBorderStyle::SingleHorizontal;
			this->dataGridView2->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::None;
			this->dataGridView2->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView2->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {
				this->dataGridViewTextBoxColumn1,
					this->dataGridViewTextBoxColumn2, this->dataGridViewTextBoxColumn3
			});
			this->dataGridView2->Cursor = System::Windows::Forms::Cursors::Default;
			this->dataGridView2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dataGridView2->GridColor = System::Drawing::SystemColors::Window;
			this->dataGridView2->Location = System::Drawing::Point(3, 3);
			this->dataGridView2->MultiSelect = false;
			this->dataGridView2->Name = L"dataGridView2";
			this->dataGridView2->ReadOnly = true;
			this->dataGridView2->RowHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::None;
			this->dataGridView2->RowHeadersVisible = false;
			this->dataGridView2->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView2->ShowCellErrors = false;
			this->dataGridView2->ShowCellToolTips = false;
			this->dataGridView2->ShowEditingIcon = false;
			this->dataGridView2->ShowRowErrors = false;
			this->dataGridView2->Size = System::Drawing::Size(768, 402);
			this->dataGridView2->TabIndex = 2;
			// 
			// dataGridViewTextBoxColumn1
			// 
			this->dataGridViewTextBoxColumn1->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
			this->dataGridViewTextBoxColumn1->HeaderText = L"TID";
			this->dataGridViewTextBoxColumn1->Name = L"dataGridViewTextBoxColumn1";
			this->dataGridViewTextBoxColumn1->ReadOnly = true;
			this->dataGridViewTextBoxColumn1->Width = 50;
			// 
			// dataGridViewTextBoxColumn2
			// 
			this->dataGridViewTextBoxColumn2->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->dataGridViewTextBoxColumn2->HeaderText = L"Start Address";
			this->dataGridViewTextBoxColumn2->MinimumWidth = 2;
			this->dataGridViewTextBoxColumn2->Name = L"dataGridViewTextBoxColumn2";
			this->dataGridViewTextBoxColumn2->ReadOnly = true;
			// 
			// dataGridViewTextBoxColumn3
			// 
			this->dataGridViewTextBoxColumn3->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
			this->dataGridViewTextBoxColumn3->FillWeight = 50;
			this->dataGridViewTextBoxColumn3->HeaderText = L"Prioridade";
			this->dataGridViewTextBoxColumn3->MinimumWidth = 2;
			this->dataGridViewTextBoxColumn3->Name = L"dataGridViewTextBoxColumn3";
			this->dataGridViewTextBoxColumn3->ReadOnly = true;
			this->dataGridViewTextBoxColumn3->Width = 79;
			// 
			// tabPage3
			// 
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Size = System::Drawing::Size(774, 408);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Módulos";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// button9
			// 
			this->button9->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->button9->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button9->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button9->Font = (gcnew System::Drawing::Font(L"Arial", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button9->Location = System::Drawing::Point(676, 5);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(118, 23);
			this->button9->TabIndex = 22;
			this->button9->TabStop = false;
			this->button9->Text = L"Close handle";
			this->button9->UseVisualStyleBackColor = false;
			this->button9->Click += gcnew System::EventHandler(this, &Handles::button1_Click);
			// 
			// Handles
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::MintCream;
			this->ClientSize = System::Drawing::Size(806, 458);
			this->Controls->Add(this->button9);
			this->Controls->Add(this->tabControl1);
			this->MinimumSize = System::Drawing::Size(822, 496);
			this->Name = L"Handles";
			this->Text = L"Handles";
			this->SizeChanged += gcnew System::EventHandler(this, &Handles::Handles_SizeChanged);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView2))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void bindingSource1_CurrentChanged(System::Object^  sender, System::EventArgs^  e) {
	}

private: System::Void checkBox1_CheckStateChanged(System::Object^  sender, System::EventArgs^  e) {
	
}

private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
	SetLastError(ERROR_SUCCESS);
	
	HANDLE hProc__z = OpenProcess(PROCESS_DUP_HANDLE, 0, processo_handle_alvo);
	
	if (hProc__z == INVALID_HANDLE_VALUE || GetLastError() == ERROR_ACCESS_DENIED)
	{
		void* Alloc__ = malloc(260);
		WCHAR* ErrorMensagem = (WCHAR*)Alloc__;
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, 0, NULL, ErrorMensagem, 260, NULL);
		MessageBoxW(0, ErrorMensagem, NULL, MB_ICONERROR);
		free(Alloc__);
	}
	HANDLE hDumpMe = INVALID_HANDLE_VALUE;
	HANDLE CloseSource;

	std::string HandleTermino;
	System::String ^ HandleAtual = dataGridView1->Rows[dataGridView1->CurrentCell->RowIndex]->Cells["Column3"]->Value->ToString();

	for (int i = 2; i < HandleAtual->Length; i++)
		HandleTermino += HandleAtual[i];

	unsigned int x;
	std::stringstream ss;
	ss << std::hex << HandleTermino.c_str();
	ss >> x;

	/*Convert de HEX para unsigned int => Fechar o identificador no processo.*/
	
	DuplicateHandle(hProc__z, (HANDLE)x, GetCurrentProcess(), &hDumpMe, 0, false, DUPLICATE_CLOSE_SOURCE);
	CloseHandle(hDumpMe);
	CloseHandle(hProc__z);
}
private: System::Void Handles_SizeChanged(System::Object^  sender, System::EventArgs^  e) {
	
	tabControl1->Height = this->Height - 62;
	tabControl1->Width = this->Width - 40;
	//dataGridView1->Height = this->Height - 80;
	//dataGridView1->Width = this->Width - 245;

	//MoveWindow((HWND)button1->Handle.ToInt32(), groupBox1->Location.X+2, dataGridView1->Location.Y + (dataGridView1->Height - button1->Height)-1,
	//	button1->Width, button1->Height, TRUE);
}
};
}