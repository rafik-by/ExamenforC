#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

struct Track
{
	char Name[50];
	char Avtorname[50];
	int sizeTrack;

	bool equals(const Track& ll) const
	{
		return(strcmp(Name, ll.Name) && strcmp(Avtorname, ll.Avtorname) && sizeTrack == ll.sizeTrack);
	}

	friend std::ostream& operator<<(std::ostream& os, const Track& corj)
	{
		os << "Имя трека" << corj.Name << "Имя автора" << corj.Avtorname << "Длительность" << corj.sizeTrack;
		return os;
	}

	friend std::istream& operator>>(std::istream& is, Track& corj)
	{
		std::cout << "Enter Track name: ";
		is.getline(corj.Name, 50);

		std::cout << "Enter Avtor name: ";
		is.getline(corj.Avtorname, 50);

		std::cout << "Enter Track size: ";
		is >> corj.sizeTrack;
		is.ignore();

		return is;
	}

};

class  Music
{
private:
	std::vector<Track> list; 

	bool contains(const Track& ll) const
	{
		for(const auto& card : list )
			if(card.equals(ll)) return true;
		return false;
	}

public:

	void add(const Track& gr)
	{
		if(!contains(gr))
		{
			list.push_back(gr);
		}
		else 
		{
			std::cout << "Danger";
		}
	
	}


	friend std::ostream& operator<<(std::ostream& os, const Music& corj)
	{
		os << "Ваш плейлист: \n";
		for(int i = 0 ; i < corj.list.size(); i++)
		{
			os <<i+1<<". " << corj.list[i] << std::endl;
		}
		return os;
	}


	friend std::istream& operator>>(std::istream& is, Music& corj)
	{ 
		int n;
		std::cout << "Число треков, которое хотите добавить: ";
		is >> n;
		is.ignore();

		for(int i = 0; i < n; i++)
		{
			Track gr;
			is >> gr;
			corj.add(gr);
		}
		return is;
	}

	void fillBinFile()
	{
		std::ofstream file("file.bin", std::ios::binary | std::ios::trunc);
		if(file.is_open())
		{
			size_t size = list.size();
			file.write(reinterpret_cast<char*>(&size), sizeof(size));
			if (size > 0) {
				file.write(reinterpret_cast<char*>(list.data()), size * sizeof(Track));
			}
			file.close();
		}

	}

	void readBinFile()
	{
		std::ifstream file("file.bin", std::ios::binary);
		if (!file.is_open()) {
			std::cout << "Danger";
			return;
		}
		size_t size = 0;
		file.read(reinterpret_cast<char*>(&size), sizeof(size));

		list.resize(size);
		if (size > 0)
		{
			file.read(reinterpret_cast<char*>(list.data()), size * sizeof(Track));
		}

		file.close();
	}

	Track longest()
	{
		int count = 0;
		int max = list[0].sizeTrack;
		for(int i = 0 ; i< list.size(); i++)
		{
			if (list[i].sizeTrack> max)
			{
				max = list[i].sizeTrack;
				count = i;

			}
			
		}
		return list[count];
	}

	int Times()
	{
		int time = 0;
		for (const auto &card : list)
		{
			time += card.sizeTrack;
		}
		return time;
	}

	Music operator+(const Music& other) const
	{
		Music result = *this;
		for(const auto& card : other.list)
		{
			if(!result.contains(card))
			result.add(card);
		}
		return result;
	}

	Music operator-(const Music& other) const
	{
		Music result;
			for(const auto& card : list)
			{
				if (!other.contains(card)) result.add(card);
			}
			return result;
	}
};

int main()
{
	setlocale(LC_ALL, "rus");

	Music pl1;
	std::cout << "=== Заполнение Плейлиста №1 ===";
	std::cin >> pl1;

	std::cout << "\n" << pl1;
	pl1.longest();
	std::cout << "Самый длинный трек:\n  " << pl1.longest() << "\n";

	std::cout << "\n=== Проверка работы с файлами ===\n";
	pl1.fillBinFile();

	Music plFromFile;
	plFromFile.readBinFile();
	std::cout << "\nПрочитанный из файла плейлист:\n" << plFromFile;

	std::cout << "\n=== Заполнение Плейлиста №2 ===\n";
	Music pl2;
	std::cin >> pl2;

	std::cout << "\n=== Оператор + (Объединение без дубликатов) ===\n";
	Music plUnion = pl1 + pl2;
	std::cout << plUnion;

	std::cout << "\n=== Оператор - (Разность: Плейлист 1 минус Плейлист 2) ===\n";
	Music plDiff = pl1 - pl2;
	std::cout << plDiff;

	return 0;
}