using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Threading;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Speech.Recognition;
using Microsoft.Speech.Synthesis;
using System.Globalization;

namespace Shooting_range
{
    public partial class MainWindow : Window
    {
        private DatabaseManager m_dbManager;
        private SpeechSynthesizer ss;
        private SpeechRecognitionEngine sre;

        private int m_dialoguePhase = 0;

        public MainWindow()
        {
            InitializeComponent();
            QuestionTextBox.Text = "Jaką operację chciałbyś wykonać?";
            AnswersTextBox.Text = "1) Poznać cennik strzelnicy.\n2) Zapisać się na strzelnicę";

            m_dbManager = new DatabaseManager();
            ss = new SpeechSynthesizer();
            ss.SetOutputToDefaultAudioDevice();
            ss.SpeakAsync("Witaj w systemie rejestracji na strzelnice. Jaką operację chciałbyś wykonać?");
            
            CultureInfo ci = new CultureInfo("pl-PL");
            sre = new SpeechRecognitionEngine(ci);
            sre.SetInputToDefaultAudioDevice();
            sre.SpeechRecognized += SpeechRecognized;
            Grammar grammar = new Grammar(".\\myGrammar.xml", "rootRule");
            grammar.Enabled = true;
            sre.LoadGrammar(grammar);
            sre.RecognizeAsync(RecognizeMode.Multiple);
        }

        private void SpeechRecognized(object sender, SpeechRecognizedEventArgs e)
        {
            
            string txt = e.Result.Text;
            float confidence = e.Result.Confidence;
            QuestionTextBox.Text = txt;
            if (confidence < 0.60)
            {
                ss.Speak("Proszę powtórzyć");
                return;
            }

            string[] words = txt.Split(' ');
            
            string number;
            try
            {
                number = e.Result.Semantics["liczby"].Value.ToString();
            }
            catch
            {
                number = "null";
            }

            string word;
            try
            {
                word = e.Result.Semantics["rejestracja"].Value.ToString();
            }
            catch
            {
                word = "null";
            }

            string word2;
            try
            {
                word2 = e.Result.Semantics["strzelanie"].Value.ToString();
            }
            catch
            {
                word2 = "null";
            }
            string word3;
            try
            {
                word3 = e.Result.Semantics["termin"].Value.ToString();
            }
            catch
            {
                word3 = "null";
            }

            AnswersTextBox.Text = word + "|" + word2 + "|" + word3 + "|" + number;



        }
    }
}
