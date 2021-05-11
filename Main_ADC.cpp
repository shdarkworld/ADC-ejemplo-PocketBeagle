// Lecutra del valor del ADC de la Beagle Pocket
// utilzando C++ (OOP)
//
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<cstring>
#include<unistd.h>
using namespace std;

#define LED3 "/sys/class/leds/beaglebone:green:usr3"	// Ruta LED3
#define ADC0 "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"		// Ruta ADC0

class LED
{
	private:
		string path;
		virtual void writeLED(string filename, string value);
		virtual void removeTrigger();
	public:
		virtual void On();
		virtual void Off();
};

class ADC
{
	private:
		virtual int lectura_ADC();
	public:
		virtual int leer();
};

void LED::writeLED(string filename, string value)
{
	// Escribe el valor proporcionado en la dirección específica
	ostringstream s;
	s << LED3;
	path = string(s.str());
	ofstream fs;
	fs.open((path + filename).c_str());
	fs << value;
	fs.close();
}

void LED::removeTrigger()
{
	// Escribe "none" en el trigger
	writeLED("/trigger", "none");
}		

void LED::On()
{
	// Enciende el LED3
	cout << "LED on" << endl;
	removeTrigger();
	writeLED("/brightness", "1");
}

void LED:: Off()
{
	// Apaga el LED3
	cout << "LED off" << endl;
	removeTrigger();
	writeLED("/brightness", "0");
}

int ADC:: lectura_ADC()
{
	// Lee y regresa el valor del ADC
	int valor;
	stringstream ss;
	ss << ADC0;
	fstream fs;
	fs.open(ss.str().c_str(), fstream::in);
	fs >> valor;
	fs.close();
	return valor;
}

int ADC:: leer()
{
	// Regresa el valor del ADC con texto
	int v_ADC = lectura_ADC();
	cout << "El valor del ADC es : " << v_ADC << endl;
	return v_ADC;
}

int main(int argc, char* argv[])
{
	// Revisa si el número de argumentos es váido
	if(argc != 2)
	{
		cout << "Número incorrecto de argumentos" << endl;
		cout << "Utilice *leer* para ejecutar una sola vez" << endl;
		cout << "Utilice *continuo* para una ejecución continua" << endl;
	}

	cout << "Ejecturando..." << endl;

	LED led; // Objeto led (Clase LED)
	ADC adc; // Objeto adc (Clase ADC)

	int val_adc;	// Valor del ADC

	// Comparando los argumentos
	if(strcmp(argv[1],"leer") == 0)		 														
	{
		// Lee una sola vez el valor del ADC y enciende el led si supera un valor de 2048
		// en caso contrario, lo apaga
		val_adc = adc.leer();
		if (val_adc > 2048)
			led.On();
		else
			led.Off();

		cout << "Ejecutado con éxito" << endl;
	}
	else
	{
		if(strcmp(argv[1], "continuo") == 0)
		{
			// Ejecuta de manera continua con un retardo de 1s entre cada lectura
			cout << "Ejecutando de manera continua..." << endl;
		        cout <<	"Utilice ctrl+c para interrumpir el programa en cualquier momento" << endl;
			cout << "Presione la tecla Enter para continuar" << endl;
			cin.ignore();

			while (1)
			{
				val_adc = adc.leer();
				if (val_adc > 2048)
					led.On();
				else
					led.Off();
				sleep(1);
			}
		}
	}

	return 0;
}

