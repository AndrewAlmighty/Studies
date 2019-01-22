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

        private String m_dateTmp = "null";
        private String m_weaponTmp = "null";
        private String m_finalOrder = "";
        private int m_finalPrice = 0;
        private int m_dialoguePhase = 0;
        private int m_tmpAmmo = 0;

        public MainWindow()
        {
            InitializeComponent();
            QuestionTextBox.Text = "Jaką operację chciałbyś wykonać?";
            AnswersTextBox.Text = "1) Poznać cennik strzelnicy.\n2) Zobaczyc dostepne bronie\n3) Zapisać się na strzelnicę";

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

        private void tellPrices()
        {
            AnswersTextBox.Text = "Wypożyczenie 1 sztuki broni - 10 zł\n Koszt 1 sztuki amunicji - 3 zł";
            ss.Speak("Oto cennik");
        }

        private void showAvailableWeapons()
        {
            AnswersTextBox.Text = "Oto lista dostępnych broni:\n-Ak\n-M4\n-Colt\n-Glock\n-CZ\nM14";
            ss.Speak("Oto lista dostępnych broni");
        }

        private String translateDate(String date)
        {
            String tmp = "";
            int day = 0;

            for (int i = 0; i < date.Length; i++)
            {
                if (date[i] != ':')
                    continue;

                else if (date[i] == ':')
                {
                    day = int.Parse(date.Substring(0, i));
                    date = date.Substring(i + 1);
                }
            }

            if (date == "january")
                date = "01";

            else if (date == "februrary")
                date = "02";

            else if (date == "march")
                date = "03";

            else if (date == "april")
                date = "04";

            else if (date == "may")
                date = "05";

            else if (date == "june")
                date = "06";

            else if (date == "july")
                date = "07";

            else if (date == "august")
                date = "08";

            else if (date == "september")
                date = "09";

            else if (date == "october")
                date = "10";

            else if (date == "november")
                date = "11";

            else if (date == "december")
                date = "12";

            if (day < 10)
                return "2019-" + date + "-0" + day.ToString();

            else 
                return "2019-" + date + "-" + day.ToString();
        }

        private int translateNumber(String num)
        {
            int number = 0;
            if (num == "null" || num == "")
                return -1;

            for (int i = 0; i < num.Length; i++)
            {
                if (num[i] == 's')
                {
                    number += 100 * (int)Char.GetNumericValue(num[i + 1]);
                    i++;
                }

                else if (num[i] == 'd')
                {
                    number += 10 * (int)Char.GetNumericValue(num[i + 1]);
                    i++;
                }

                else if (num[i] == 'n')
                {
                    String tmp = "";
                    tmp += num[i + 1];
                    tmp += num[i + 2];
                    i += 2;
                    number += Int32.Parse(tmp);
                }

                else if (num[i] == 'j')
                {
                    number += (int)Char.GetNumericValue(num[i + 1]);
                    i++;
                }

            }

            return number;
        }

        private String prepareWeaponAndAmmoString(String shooting)
        {
            String tmp = "";
            m_tmpAmmo = 0;

            for (int i = 0; i < shooting.Length; i++)
            {
                if (shooting[i] != ':')
                    tmp += shooting[i];

                else if (shooting[i] == ':')
                {
                    m_tmpAmmo = translateNumber(tmp);

                    if (m_tmpAmmo > 0)
                        tmp = shooting.Substring(i + 1);

                    else
                    {
                        m_tmpAmmo = translateNumber(shooting.Substring(i + 1));
                        tmp = shooting.Substring(0, i);
                    }

                    break;
                }
            }

            tmp = "Broń :" + tmp + ", " + m_tmpAmmo.ToString() + " sztuk amunicji";
            return tmp;
        }

        private void handleDialogue(string number, string register, string shooting, string date)
        {
            if (m_dialoguePhase == 0)
            {
                if (register == "price-list")
                    tellPrices();

                else if (register == "weapon-list")
                    showAvailableWeapons();

                else if (register == "register")
                {
                    m_dialoguePhase = 1;
                    AnswersTextBox.Text = "";
                    QuestionTextBox.Text = "Podaj datę wizyty na strzelnicy";
                    ss.Speak("Podaj termin wizyty na strzelnicy");
                }
            }

            else if (m_dialoguePhase == 1)
            {
                if (date != "null")
                {
                    m_dialoguePhase = 2;
                    m_dateTmp = date;
                    AnswersTextBox.Text = "";
                    QuestionTextBox.Text = "Strzelania rozpoczynają się o równej godzinie.Proszę podać godzinę";
                    ss.Speak("Strzelania rozpoczynają się o równej godzinie. Proszę podać godzinę");
                }
            }

            else if (m_dialoguePhase == 2)
            {
                if (number != "null")
                {
                    int tmp = translateNumber(number);
                    m_dateTmp = translateDate(m_dateTmp);
                    AnswersTextBox.Text = m_dateTmp + ":" + tmp.ToString();

                    if (m_dbManager.checkIfDayAndHourIsFree(m_dateTmp, tmp))
                    {
                        m_dbManager.insertToTimeTable(m_dateTmp, tmp);
                        m_dateTmp = "null";
                        m_dialoguePhase = 3;
                        AnswersTextBox.Text = "";
                        QuestionTextBox.Text = "Z jakiej broni chcesz strzelać?";
                        ss.Speak("Z jakiej broni chcesz strzelać");
                    }

                    else
                    {
                        ss.Speak("Podany termin jest zajęty");
                        AnswersTextBox.Text = "";
                        QuestionTextBox.Text = "Podaj datę wizyty na strzelnicy";
                        ss.Speak("Podaj termin wizyty na strzelnicy");
                        m_dateTmp = "null";
                        m_dialoguePhase = 1;
                    }
                }
            }

            else if (m_dialoguePhase == 3)
            {
                if (shooting != "null")
                {
                    m_dialoguePhase = 5;
                    m_weaponTmp = prepareWeaponAndAmmoString(shooting);
                    QuestionTextBox.Text = "Czy potwierdzasz chęć strzelania z poniższej broni i ilość amunicji?";
                    AnswersTextBox.Text = m_weaponTmp;
                    ss.Speak("Czy potwierdzasz chęć strzelania z poniższej broni i ilość amunicji");
                }

                else if (register != "null")
                {
                    m_dialoguePhase = 4;
                    m_weaponTmp = register;
                    QuestionTextBox.Text = "Podaj ilość amunicji.";
                    AnswersTextBox.Text = "";
                    ss.Speak("Wybrano broń " + register + ". Ile sztuk amunicji do tego");
                }
            }

            else if (m_dialoguePhase == 4)
            {
                if (number != "null")
                {
                    m_dialoguePhase = 5;
                    m_weaponTmp = prepareWeaponAndAmmoString(m_weaponTmp + ":" + number);
                    QuestionTextBox.Text = "Czy potwierdzasz chęć strzelania z poniższej broni i ilość amunicji?";
                    AnswersTextBox.Text = m_weaponTmp;
                    ss.Speak("Czy potwierdzasz chęć strzelania z poniższej broni i ilość amunicji");
                }
            }

            else if (m_dialoguePhase == 5)
            {
                if (register == "yes")
                {
                    String tmp = "";
                    bool gotWeapon = false;
                    for (int i = 0; i < m_weaponTmp.Length; i++)
                    {
                        if (!gotWeapon && m_weaponTmp[i] == ':')
                            gotWeapon = true;

                        else if (gotWeapon && m_weaponTmp[i] != ',')
                            tmp += m_weaponTmp[i];

                        else if (gotWeapon && m_weaponTmp[i] == ',')
                            break;
                    }

                    int tmpAmmo = m_dbManager.getAmmoQuantity(tmp);

                    if (tmpAmmo < m_tmpAmmo)
                    {
                        m_dialoguePhase = 3;
                        QuestionTextBox.Text = "Brak wystarczająco amunicji na składzie. Proszę o ponowny wybór broni.";
                        ss.Speak("Brak wystarczająco amunicji na składzie. Proszę o ponowny wybór broni.");
                        m_weaponTmp = "null";
                        m_tmpAmmo = 0;
                        AnswersTextBox.Text = "";
                    }

                    else
                    {
                        m_dialoguePhase = 6;
                        m_finalPrice += 10 + (3 * m_tmpAmmo);
                        m_finalOrder += m_weaponTmp + "\n";
                        tmpAmmo -= m_tmpAmmo;
                        m_dbManager.updateAmmoQuantity(tmp, tmpAmmo);
                        QuestionTextBox.Text = "Dodano zestaw do zamówienia. Zakończyć wybór broni?";
                        ss.Speak("Dodano zestaw do zamówienia. Zakończyć czy dodać nową broń");
                        m_weaponTmp = "null";
                        m_tmpAmmo = 0;
                        AnswersTextBox.Text = "Zamówienie:\n" + m_finalOrder + "Koszt:" + m_finalPrice;
                    }
                }

                if (register == "no")
                {
                    QuestionTextBox.Text = "Z jakiej broni chcesz strzelać?";
                    ss.Speak("Z jakiej broni chcesz strzelać");
                    m_dialoguePhase = 3;
                    m_weaponTmp = "null";
                    m_tmpAmmo = 0;
                }
            }

            else if (m_dialoguePhase == 6)
            {
                if (register == "yes" || register == "finish")
                {
                    m_dialoguePhase = 0;
                    QuestionTextBox.Text = "Jaką operację chciałbyś wykonać?";
                    AnswersTextBox.Text = "1) Poznać cennik strzelnicy.\n2) Poznać dostępne bronie\n3) Zapisać się na strzelnicę";
                    m_weaponTmp = "null";
                    m_tmpAmmo = 0;
                    m_finalOrder = "";
                    m_finalPrice = 0;
                }

                else if (register == "no" || register == "again")
                {
                    m_dialoguePhase = 3;
                    m_weaponTmp = "null";
                    m_tmpAmmo = 0;
                    QuestionTextBox.Text = "Z jakiej broni chcesz strzelać?";
                    ss.Speak("Z jakiej broni chcesz strzelać");
                }
            }            
        }

        private void SpeechRecognized(object sender, SpeechRecognizedEventArgs e)
        {            
            string txt = e.Result.Text;
            float confidence = e.Result.Confidence;
            //QuestionTextBox.Text = txt;
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
                number = number.Substring(15);
            }
            catch
            {
                number = "null";
            }

            string register;
            try
            {
                register = e.Result.Semantics["rejestracja"].Value.ToString();
            }
            catch
            {
                register = "null";
            }

            string shooting;
            try
            {
                shooting = e.Result.Semantics["strzelanie"].Value.ToString();
                shooting = shooting.Substring(16);
                if (shooting[0] == '[')
                    shooting = shooting.Substring(15);

                else
                {
                    for (int i = 1; i < shooting.Length; i++)
                    {
                        if (shooting[i] == '[')
                        {
                            shooting = shooting.Substring(0, i - 1) + ":" + shooting.Substring(i + 15);
                            break;
                        }
                    }
                }
            }
            catch
            {
                shooting = "null";
            }
            string date;
            try
            {
                date = e.Result.Semantics["termin"].Value.ToString();
                date = date.Substring(16);
            }
            catch
            {
                date = "null";
            }

           // AnswersTextBox.Text = prepareWeaponAndAmmoString(shooting);
           handleDialogue(number, register, shooting, date);
        }
    }
}
