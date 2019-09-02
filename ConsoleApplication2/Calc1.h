#pragma once
#include <Windows.h>
#include <iostream>
#include <string>

namespace ConsoleApplication2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	/// <summary>
	/// Summary for Calc
	/// </summary>
	public ref class Calc : public System::Windows::Forms::Form
	{
	public:
		Calc(void)
		{
			InitializeComponent();
			FocusOnTextBox();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Calc()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::Button^  button1;
	protected:
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Button^  button8;
	private: System::Windows::Forms::Button^  button9;
	private: System::Windows::Forms::Button^  button10;
	private: System::Windows::Forms::Button^  button11;
	private: System::Windows::Forms::Button^  button12;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button13;
	private: System::Windows::Forms::Button^  button14;
	private: System::Windows::Forms::Button^  button15;
	private: System::Windows::Forms::Button^  button16;
	private: System::Windows::Forms::Button^  button17;
	private: System::Windows::Forms::Button^  button18;
	private: System::Windows::Forms::Button^  button19;
	private: System::Windows::Forms::TextBox^  textBox1;

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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->button10 = (gcnew System::Windows::Forms::Button());
			this->button11 = (gcnew System::Windows::Forms::Button());
			this->button12 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button13 = (gcnew System::Windows::Forms::Button());
			this->button14 = (gcnew System::Windows::Forms::Button());
			this->button15 = (gcnew System::Windows::Forms::Button());
			this->button16 = (gcnew System::Windows::Forms::Button());
			this->button17 = (gcnew System::Windows::Forms::Button());
			this->button18 = (gcnew System::Windows::Forms::Button());
			this->button19 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button1->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button1->FlatAppearance->BorderSize = 2;
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(12, 195);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(108, 30);
			this->button1->TabIndex = 0;
			this->button1->Text = L"0";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button2->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button2->FlatAppearance->BorderSize = 2;
			this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button2->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button2->Location = System::Drawing::Point(69, 159);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(51, 30);
			this->button2->TabIndex = 1;
			this->button2->Text = L"2";
			this->button2->UseVisualStyleBackColor = false;
			this->button2->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button3
			// 
			this->button3->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button3->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button3->FlatAppearance->BorderSize = 2;
			this->button3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button3->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button3->Location = System::Drawing::Point(12, 159);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(51, 30);
			this->button3->TabIndex = 2;
			this->button3->Text = L"1";
			this->button3->UseVisualStyleBackColor = false;
			this->button3->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button5
			// 
			this->button5->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button5->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button5->FlatAppearance->BorderSize = 2;
			this->button5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button5->Font = (gcnew System::Drawing::Font(L"Arial", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button5->Location = System::Drawing::Point(126, 195);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(51, 30);
			this->button5->TabIndex = 4;
			this->button5->Text = L",";
			this->button5->UseVisualStyleBackColor = false;
			this->button5->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button6
			// 
			this->button6->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button6->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button6->FlatAppearance->BorderSize = 2;
			this->button6->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button6->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button6->Location = System::Drawing::Point(126, 86);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(51, 30);
			this->button6->TabIndex = 5;
			this->button6->Text = L"9";
			this->button6->UseVisualStyleBackColor = false;
			this->button6->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button7
			// 
			this->button7->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button7->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button7->FlatAppearance->BorderSize = 2;
			this->button7->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button7->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button7->Location = System::Drawing::Point(69, 86);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(51, 30);
			this->button7->TabIndex = 6;
			this->button7->Text = L"8";
			this->button7->UseVisualStyleBackColor = false;
			this->button7->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button8
			// 
			this->button8->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button8->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button8->FlatAppearance->BorderSize = 2;
			this->button8->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button8->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button8->Location = System::Drawing::Point(12, 86);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(51, 30);
			this->button8->TabIndex = 7;
			this->button8->Text = L"7";
			this->button8->UseVisualStyleBackColor = false;
			this->button8->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button9
			// 
			this->button9->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button9->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button9->FlatAppearance->BorderSize = 2;
			this->button9->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button9->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button9->Location = System::Drawing::Point(126, 123);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(51, 30);
			this->button9->TabIndex = 8;
			this->button9->Text = L"6";
			this->button9->UseVisualStyleBackColor = false;
			this->button9->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button10
			// 
			this->button10->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button10->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button10->FlatAppearance->BorderSize = 2;
			this->button10->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button10->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button10->Location = System::Drawing::Point(69, 123);
			this->button10->Name = L"button10";
			this->button10->Size = System::Drawing::Size(51, 30);
			this->button10->TabIndex = 9;
			this->button10->Text = L"5";
			this->button10->UseVisualStyleBackColor = false;
			this->button10->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button11
			// 
			this->button11->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button11->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button11->FlatAppearance->BorderSize = 2;
			this->button11->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button11->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button11->Location = System::Drawing::Point(12, 123);
			this->button11->Name = L"button11";
			this->button11->Size = System::Drawing::Size(51, 30);
			this->button11->TabIndex = 10;
			this->button11->Text = L"4";
			this->button11->UseVisualStyleBackColor = false;
			this->button11->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button12
			// 
			this->button12->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button12->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button12->FlatAppearance->BorderSize = 2;
			this->button12->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button12->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button12->Location = System::Drawing::Point(126, 159);
			this->button12->Name = L"button12";
			this->button12->Size = System::Drawing::Size(51, 30);
			this->button12->TabIndex = 11;
			this->button12->Text = L"3";
			this->button12->UseVisualStyleBackColor = false;
			this->button12->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button4
			// 
			this->button4->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button4->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button4->FlatAppearance->BorderSize = 2;
			this->button4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button4->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button4->Location = System::Drawing::Point(12, 50);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(51, 30);
			this->button4->TabIndex = 12;
			this->button4->Text = L"CE";
			this->button4->UseVisualStyleBackColor = false;
			this->button4->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button13
			// 
			this->button13->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button13->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button13->FlatAppearance->BorderSize = 2;
			this->button13->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button13->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button13->Location = System::Drawing::Point(69, 50);
			this->button13->Name = L"button13";
			this->button13->Size = System::Drawing::Size(51, 30);
			this->button13->TabIndex = 13;
			this->button13->Text = L"C";
			this->button13->UseVisualStyleBackColor = false;
			this->button13->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button14
			// 
			this->button14->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button14->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button14->FlatAppearance->BorderSize = 2;
			this->button14->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button14->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button14->Location = System::Drawing::Point(126, 50);
			this->button14->Name = L"button14";
			this->button14->Size = System::Drawing::Size(51, 30);
			this->button14->TabIndex = 14;
			this->button14->Text = L"←";
			this->button14->UseVisualStyleBackColor = false;
			this->button14->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button15
			// 
			this->button15->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button15->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button15->FlatAppearance->BorderSize = 2;
			this->button15->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button15->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button15->Location = System::Drawing::Point(183, 195);
			this->button15->Name = L"button15";
			this->button15->Size = System::Drawing::Size(51, 30);
			this->button15->TabIndex = 15;
			this->button15->Text = L"=";
			this->button15->UseVisualStyleBackColor = false;
			this->button15->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button16
			// 
			this->button16->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button16->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button16->FlatAppearance->BorderSize = 2;
			this->button16->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button16->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button16->Location = System::Drawing::Point(183, 159);
			this->button16->Name = L"button16";
			this->button16->Size = System::Drawing::Size(51, 30);
			this->button16->TabIndex = 16;
			this->button16->Text = L"+";
			this->button16->UseVisualStyleBackColor = false;
			this->button16->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button17
			// 
			this->button17->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button17->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button17->FlatAppearance->BorderSize = 2;
			this->button17->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button17->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button17->Location = System::Drawing::Point(183, 123);
			this->button17->Name = L"button17";
			this->button17->Size = System::Drawing::Size(51, 30);
			this->button17->TabIndex = 17;
			this->button17->Text = L"-";
			this->button17->UseVisualStyleBackColor = false;
			this->button17->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button18
			// 
			this->button18->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button18->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button18->FlatAppearance->BorderSize = 2;
			this->button18->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button18->Font = (gcnew System::Drawing::Font(L"Calibri", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button18->Location = System::Drawing::Point(183, 86);
			this->button18->Name = L"button18";
			this->button18->Size = System::Drawing::Size(51, 30);
			this->button18->TabIndex = 18;
			this->button18->Text = L"*";
			this->button18->UseVisualStyleBackColor = false;
			this->button18->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// button19
			// 
			this->button19->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->button19->FlatAppearance->BorderColor = System::Drawing::Color::Gray;
			this->button19->FlatAppearance->BorderSize = 2;
			this->button19->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button19->Font = (gcnew System::Drawing::Font(L"Calibri", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button19->Location = System::Drawing::Point(183, 50);
			this->button19->Name = L"button19";
			this->button19->Size = System::Drawing::Size(51, 30);
			this->button19->TabIndex = 19;
			this->button19->Text = L"/";
			this->button19->UseVisualStyleBackColor = false;
			this->button19->Click += gcnew System::EventHandler(this, &Calc::button1_Click);
			// 
			// textBox1
			// 
			this->textBox1->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->textBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->textBox1->Location = System::Drawing::Point(12, 12);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(222, 26);
			this->textBox1->TabIndex = 20;
			this->textBox1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Calc::textBox1_KeyPress);
			// 
			// Calc
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlLight;
			this->ClientSize = System::Drawing::Size(248, 234);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button19);
			this->Controls->Add(this->button18);
			this->Controls->Add(this->button17);
			this->Controls->Add(this->button16);
			this->Controls->Add(this->button15);
			this->Controls->Add(this->button14);
			this->Controls->Add(this->button13);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button12);
			this->Controls->Add(this->button11);
			this->Controls->Add(this->button10);
			this->Controls->Add(this->button9);
			this->Controls->Add(this->button8);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"Calc";
			this->Text = L"Calculadora";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
		double TempResult(double __Number1, char Operator, double __Number2)
		{
			if (Operator == '+')
				return __Number1 + __Number2;
			if (Operator == '-')
				return __Number1 - __Number2;
			if (Operator == '/')
				return __Number1 / __Number2;
			if (Operator == '*')
				return __Number1 * __Number2;

			return 0;
		}

		System::Void FocusOnTextBox()
		{
			textBox1->Select();
			textBox1->Select(textBox1->Text->Length, textBox1->Text->Length);
		}

		System::Void AddToTextBox(String^ Text)
		{
			if (textBox1->Text->Contains("="))
				textBox1->Text = String::Empty;

			if (!Char::IsNumber(Convert::ToChar(Text)) && Text != "=")
			{
				if (textBox1->TextLength == 0)
				{
					if (Text == "-")
					{
						textBox1->Text += Text;
						FocusOnTextBox();
					}
					return;
				}
				else
				{
					if (textBox1->Text[textBox1->TextLength - 1] == ',')
						return;
					else if (!Char::IsNumber(textBox1->Text[textBox1->TextLength - 1]))
					{
						textBox1->Text = textBox1->Text->Substring(0, textBox1->TextLength - 1);
						textBox1->Text += Text;
						FocusOnTextBox();
						return;
					}
				}
			}

			if (Text == gcnew String("="))
			{
				if (textBox1->TextLength <= 0)
					return;

				if (!Char::IsNumber(textBox1->Text[textBox1->TextLength - 1]) || 
					!textBox1->Text->Contains("+") && !textBox1->Text->Contains("-") && !textBox1->Text->Contains("*")
					&& !textBox1->Text->Contains("/"))
				{
					FocusOnTextBox();
					return;
				}

				textBox1->Text += Text;

				array<String^>^ numbers = gcnew array<String^>(1024);
				array<Char^>^ operato = gcnew array<Char^>(1024);

				double resultado, number1, number2;
				int Cont = 0;

				for (int i = 0; i < textBox1->Text->Length; i++)
				{
					if (Char::IsNumber(textBox1->Text[i]))
						numbers[Cont] += textBox1->Text[i];
					else if (textBox1->Text[i] == ',')
						numbers[Cont] += ".";
					else
					{
						if (textBox1->Text[i] != '=')
						{
							operato[Cont] = textBox1->Text[i];
							Cont++;
						}
					}
				}

				for (int x = 0; x < Cont; x++)
				{
					number1 = Convert::ToDouble(numbers[x], System::Globalization::CultureInfo::InvariantCulture);
					number2 = Convert::ToDouble(numbers[x + 1], System::Globalization::CultureInfo::InvariantCulture);

					char __OpChar = (char)Convert::ToChar(operato[x]);

					if (x == 0)
						resultado = TempResult(number1, __OpChar, number2);
					else
						resultado = TempResult(resultado, __OpChar, number2);
				}
				textBox1->Text += resultado;
				FocusOnTextBox();
				return;
			}

			textBox1->Text += Text;
			FocusOnTextBox();
		}

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		if (sender->ToString()[sender->ToString()->Length - 1] == 'C')
		{
			textBox1->Text = String::Empty;
			FocusOnTextBox();
			return;
		}
		else if (sender->ToString()[sender->ToString()->Length - 1] == 'E')
		{
			if (textBox1->TextLength > 1)
			{
				for (int i = textBox1->TextLength - 1; i > 0; i--)
				{
					if (!Char::IsNumber(textBox1->Text[i]) && textBox1->Text[i] != ',')
					{
						textBox1->Text = textBox1->Text->Substring(0, i + 1);
						FocusOnTextBox();
						return;
					}
				}
			}
			FocusOnTextBox();
			return;
		}

		//button1_Click(button1, new MouseEventArgs(System.Windows.Forms.MouseButtons.Left, 1, 1, 1, 1));

		if ((char)sender->ToString()[sender->ToString()->Length - 1] == (int)-112)
		{
			if(textBox1->TextLength > 0 )
				textBox1->Text = textBox1->Text->Substring(0, textBox1->Text->Length - 1);
			FocusOnTextBox();
			return;
		}
		AddToTextBox(sender->ToString()[sender->ToString()->Length - 1].ToString());
	}


private: System::Void textBox1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {

	e->Handled = true;
		
	if (Char::IsNumber(e->KeyChar))
			AddToTextBox(e->KeyChar.ToString());
		else if (e->KeyChar == 13 || e->KeyChar == '=')
		{
			if (button16->Enabled)
				AddToTextBox(gcnew String("="));
		}
		else if (e->KeyChar == '+' || e->KeyChar == '-' || e->KeyChar == '/' || e->KeyChar == '*' || e->KeyChar == ',')
			AddToTextBox(e->KeyChar.ToString());
		else if (e->KeyChar == '.')
			AddToTextBox(",");
		else if (textBox1->Text->Length > 0 && e->KeyChar == 8)
		{
			textBox1->Text = textBox1->Text->Substring(0, textBox1->TextLength - 1);
			textBox1->Select(textBox1->Text->Length, textBox1->TextLength);
		}
		else if (e->KeyChar == 27)
			textBox1->Text = String::Empty;
}
};
}
