using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections;

namespace WinFormsApp1
{
    public partial class Bank : Form
    {
        public Debts plati = new Debts();

        public class Debt
        {
            public int Euros { get; set; }
            public int Cents { get; set; }
            public int ID { get; set; }
            public DateTime Data { get; set; }
            public string Reason { get; set; }
            public Debt(int Eur, int Cen, int Id, DateTime Datka, string Reson){
                Euros = Eur;
                Cents = Cen;
                ID = Id; 
                Data = Datka;
                Reason = Reson;
            }
        }
        public class Debts : ArrayList, ITypedList
        {
            public PropertyDescriptorCollection GetItemProperties(PropertyDescriptor[] listAccessors)
            {
                return TypeDescriptor.GetProperties(typeof(Debt));
            }
            public string GetListName(PropertyDescriptor[] listAccessors)
            {
                return "Your debts";
            }
        }
        public Bank()
        {
            InitializeComponent();
            plati.Add(new Debt(105, 24, 1, new DateTime(2024, 2, 12), "I don't work now...")); 
            plati.Add(new Debt(341, 6, 2, new DateTime(2027, 10, 25), "I have already one debt!.."));
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            dataGridView1.DataSource = plati;
            numericUpDown1.DataBindings.Add("Text", plati, "Reason");
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            Debt d = new Debt(int.Parse(textBox2.Text), (int)numericUpDown1.Value, int.Parse(textBox3.Text), dateTimePicker1.Value, textBox1.Text); 
            plati.Add(d);
            (BindingContext[plati] as CurrencyManager).Refresh();
        }

        private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label3_Click(object sender, EventArgs e)
        {

        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged_1(object sender, EventArgs e)
        {

        }
    }
}
