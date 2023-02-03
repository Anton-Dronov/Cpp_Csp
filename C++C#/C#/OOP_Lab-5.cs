using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.Threading.Tasks;
using static Hrantan.Dispatcher;

namespace Hrantan
{
    public class Wallet
    {
        public readonly long eur;
        public readonly char cen;
        protected long euros;
        protected char cents;
        public long Euros
        {
            get { return euros; }
            set { euros = value; }
        }
        public int Cents
        {
            get { return cents; }
            set { cents = (char)value; }
        }
        public virtual void Info() {
            Console.WriteLine("{0}.{1}$", euros, cents);
        }
    }

    public class Money : Wallet
    {
        public Money(long eur, char cen)
        {
            this.euros = eur;
            this.cents = cen;
        }
        public override void Info()
        {
            Console.WriteLine("Your balance is: {0}.{1}$", euros, (int)cents);
        }
        public void Addition(double Oper)
        {
            euros += (long)Math.Floor(Oper);
            cents += (char)Math.Floor((Oper - Math.Floor(Oper)) * 100);
            if ((int)cents >= 100)
            {
                euros++;
                cents = (char)((int)cents - 100);
            }
        }
        public void Subtraction(double Oper)
        {
            euros -= (long)Math.Floor(Oper);
            int deltaCents = (int)Math.Floor((Oper - Math.Floor(Oper)) * 100);
            if ((int)cents < deltaCents)
            {
                euros--;
                cents = (char)((int)cents + 100);
            }
            cents -= (char)deltaCents;
        }
        public void Division(int Oper)
        {
            int deltaEuros = (int)euros;
            euros /= Oper;
            cents = (char)(((int)cents + (deltaEuros - euros * Oper) * 100) / Oper);
        }
        public void Division(double Oper)
        {
            double Rez = 0;
            Rez = (double)euros / Oper + (double)cents / (Oper * 100);
            euros = (int)Math.Floor(Rez);
            cents = (char)((Rez - Math.Floor(Rez)) * 100);
        }
        public void Multiplication(double Oper)
        {
            double Rez = 0;
            Rez = (double)euros * Oper + (double)cents * Oper / 100;
            euros = (int)Math.Floor(Rez);
            cents = (char)((Rez - Math.Floor(Rez)) * 100);
        }
        public int Comparison(double Oper)
        {
            int ifka = 0;
            if (euros < Math.Floor(Oper) || (euros == (int)Math.Floor(Oper) && (double)cents / 100 < (Oper - Math.Floor(Oper))))
            {
                ifka = -1;
            }
            else if (euros > Math.Floor(Oper) || (euros == (int)Math.Floor(Oper) && (double)cents / 100 > Math.Round((Oper - Math.Floor(Oper)) * 100) / 100))
            {
                ifka = 1;
            }
            else
            {
                ifka = 0;
            }
            return ifka;
        }
        public int Comparison(long ee, char cc)
        {
            int ifka = 0;
            if (euros < ee || (euros == ee && (int)cents < (int)cc))
            {
                ifka = -1;
            }
            else if (euros > ee || (euros == ee && (int)cents > (int)cc))
            {
                ifka = 1;
            }
            else
            {
                ifka = 0;
            }
            return ifka;
        }
    }

    [Serializable]
    internal class BankException : Exception
    {
        public BankException() { }
        public BankException(string massage) : base(massage) { }
        public BankException(string massage, Exception WTF) : base(massage, WTF) { }
        protected BankException(SerializationInfo info, StreamingContext context) :
        base(info, context) { }
    }

    public class Debt : Wallet
    {
        private int id;
        public Debt(long eur, char cen, int Id)
        {
            this.euros = eur;
            this.cents = cen;
            this.id = Id;
        }
        public Debt(int Id)
        {
            this.euros = 0;
            this.cents = (char)0;
            this.id = Id;
        }
        public int ID
        {
            get { return id; }
        }
        new public void Info()
        {
            Console.WriteLine("You must pay {0} euros and {1} cents.", euros, (int)cents);
        }
    }

    public class StartTest
    {
        public static void Main()
        {
            try
            {
                Console.WriteLine("Anton, Dronov, 4103BDA");
                Console.WriteLine(DateTime.Now);
                Money balance = new Money(55, (char)96);
                Debt plati1 = new Debt(2, (char)28, 29576);
                Debt plati2 = new Debt(13, (char)37, 6378);
                Debt plati3 = new Debt(524);
                plati3.Euros = 320;
                plati3.Cents = 02;
                try
                {
                    Dispatcher d = new Dispatcher();
                    d.ShowDebtEvent += new TryDebt(plati1.Info);
                    d.ShowDebtEvent += new TryDebt(plati2.Info);
                    d.ShowDebtEvent += new TryDebt(plati3.Info);
                    d.ShowDebtCommand();
                } catch (BankException BAN)
                {
                    Console.WriteLine(BAN);
                }
                double wantToAdd;
                double wantToSubtract;
                int wantToDivideInt;
                double wantToDivideDouble;
                double wantToMultiDouble;
                double wantToCompare;
                balance.Info();

                wantToAdd = 19.75;
                Console.WriteLine("{0},{1} + " + wantToAdd + ";", balance.Euros, balance.Cents);
                balance.Addition(wantToAdd);
                balance.Info();

                wantToSubtract = 34.88;
                Console.WriteLine("{0},{1} - " + wantToSubtract + ";", balance.Euros, balance.Cents);
                balance.Subtraction(wantToSubtract);
                balance.Info();

                wantToDivideInt = 3;
                Console.WriteLine("{0},{1} / " + wantToDivideInt + ";", balance.Euros, balance.Cents);
                balance.Division(wantToDivideInt);
                balance.Info();

                wantToDivideDouble = 5.11;
                Console.WriteLine("{0},{1} / " + wantToDivideDouble + ";", balance.Euros, balance.Cents);
                balance.Division(wantToDivideDouble);
                balance.Info();

                wantToMultiDouble = 34.71;
                Console.WriteLine("{0},{1} * " + wantToMultiDouble + ";", balance.Euros, balance.Cents);
                balance.Multiplication(wantToMultiDouble);
                balance.Info();

                wantToCompare = 380.07;
                Console.WriteLine("{0},{1} ? " + wantToCompare + ";", balance.Euros, balance.Cents);
                int comp = balance.Comparison(wantToCompare);
                switch (comp)
                {
                    case -1:
                        Console.WriteLine("Your balance is less than compared number");
                        break;
                    case 0:
                        Console.WriteLine("Your balance is equal to compared number");
                        break;
                    case 1:
                        Console.WriteLine("Your balance is more than compared number");
                        break;
                    default:
                        Console.WriteLine("Error!!!");
                        break;
                }
                int debtID = 0;
                for (int i = 0; i < 3; i++)
                {
                    switch (i)
                    {
                        case 0:
                            comp = balance.Comparison(plati1.Euros, (char)plati1.Cents);
                            debtID = plati1.ID;
                            break;
                        case 1:
                            comp = balance.Comparison(plati2.Euros, (char)plati2.Cents);
                            debtID = plati2.ID;
                            break;
                        case 2:
                            comp = balance.Comparison(plati3.Euros, (char)plati3.Cents);
                            debtID = plati3.ID;
                            break;
                        default:
                            Console.WriteLine("Error!!!");
                            break;
                    }
                    if (comp >= 0)
                    {
                        Console.WriteLine("You can pay your debt #{0}.", debtID);
                    }
                    else
                    {
                        Console.WriteLine("You can't pay your debt #{0}!", debtID);
                    }
                }
            } catch (Exception e)
            {
                Console.WriteLine(e);
            }
            finally
            {
                Console.WriteLine("(This text will always be shown!)");
            }
            Console.WriteLine("Press Enter...");
            Console.ReadLine();
        }
    }
    public class Dispatcher{
        public delegate void TryDebt();
        public event TryDebt ShowDebtEvent;
        public void ShowDebtCommand()
        {
            if (ShowDebtEvent != null)
            {
                ShowDebtEvent();
            }
        }
    }
}