#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdio>

// std::cin clearing
#define CIN_FLUSH if (std::cin.peek()) std::cin.ignore()

class Song {
public:
	unsigned id = 0;
	char title[32] = {};
	unsigned album_id = 0;
	char genre[12] = {};
	unsigned duration = 0;
	char audiocodec[5] = {};
	unsigned count_of_listens = 0;
	int getNewID(void);
};

class Album {
public:
	unsigned id = 0;
	unsigned singer_id = 0;
	char title[32] = {};
	unsigned year_of_release = 0;
	int getNewID(void);
};

class Singer {
public:
	unsigned id = 0;
	char name[32] = {};
	char years[10] = {};
	char country[16] = {};
	int getNewID(void);
};

// Vectors, which store information about objects 
std::vector<Song*> Songs;
std::vector<Album*> Albums;
std::vector<Singer*> Singers;

// Binary files, in which is stored an information about objects 
std::string Singers_fname{ "Singers.bin" };
std::string Albums_fname{ "Albums.bin" };
std::string Songs_fname{ "Songs.bin" };

// These vectors are used for filtering and sorting elements
std::vector<unsigned> foundSongs;
std::vector<unsigned> foundAlbums;
std::vector<unsigned> foundSingers;

// Getting ID for songs
unsigned maxSongID = 0;
int Song::getNewID() {
	for (unsigned i = 0; i < Songs.size(); i++) {
		if (Songs[i]->id > maxSongID) {
			maxSongID = Songs[i]->id;
		}
	}
	maxSongID++;
	return maxSongID;
}

// Getting ID for albums
unsigned maxAlbumID = 0;
int Album::getNewID() {
	for (unsigned i = 0; i < Albums.size(); i++) {
		if (Albums[i]->id > maxAlbumID) {
			maxAlbumID = Albums[i]->id;
		}
	}
	maxAlbumID++;
	return maxAlbumID;
}

// Getting ID for singers
unsigned maxSingerID = 0;
int Singer::getNewID() {
	for (unsigned i = 0; i < Singers.size(); i++) {
		if (Singers[i]->id > maxSingerID) {
			maxSingerID = Singers[i]->id;
		}
	}
	maxSingerID++;
	return maxSingerID;
}

// This function reads defined count of symbols
void readChars(char* dest, unsigned count) {
	std::string readin;
	std::cin.clear();
	std::getline(std::cin, readin);
	for (unsigned i = 0; (i < count-1) && (i < readin.length()); i++) {
		dest[i] = readin[i];
	}
}

// Checking whether entered data is integer
unsigned getInteger() {
	unsigned ifint = 0;
	std::string check;
	while (true) {
		std::cin >> check;
		try {
			ifint = std::stoi(check);
			return ifint;
		}
		catch (...){
			std::cin.clear();
			std::cin.ignore();
			std::cout << "Incorrect input! Try again:			";
		}
	}
	return 0;
}

// Songs creating
void createSongs(void) {
	std::cout << "\nEnter data about song.\n"
				<< "Once you've finished, enter /STOP.\n\n";
	while (true) {
		Song* song = new Song();
		std::cout << "Song title:        ";
		CIN_FLUSH;
		readChars(song->title, sizeof(song->title));
		if (std::string{ song->title } == "/STOP")
			break;
		std::cout << "Album ID:       ";
		song->album_id = getInteger();
		CIN_FLUSH;
		std::cout << "Genre:         ";
		readChars(song->genre, sizeof(song->genre));
		std::cout << "Song duration (in seconds):      ";
		song->duration = getInteger();
		CIN_FLUSH;
		std::cout << "Audiocodec:           ";
		readChars(song->audiocodec, sizeof(song->audiocodec));
		std::cout << "Count of listenings:      ";
		song->count_of_listens = getInteger();
		std::cout << '\n';
		song->id = song->getNewID();
		Songs.push_back(song);
	}
}

// Albums creating
void createAlbums(void) {
	std::cout << "\nEnter data about album.\n"
				<< "Once you've finished, enter /STOP.\n\n";
	while (true) {
		Album* album = new Album();
		std::cout << "Album title:        ";
		CIN_FLUSH;
		readChars(album->title, sizeof(album->title));
		if (std::string{ album->title } == "/STOP")
			break;
		std::cout << "Singer ID:       ";
		album->singer_id = getInteger();
		std::cout << "Year of release:      ";
		album->year_of_release = getInteger();
		std::cout << '\n';
		album->id = album->getNewID();
		Albums.push_back(album);
	}
}

// Singers creating
void createSingers(void) {
	std::cout << "\nEnter data about singer.\n"
				<< "Once you've finished, enter /STOP.\n\n";
	CIN_FLUSH;
	while (true) {
		Singer* singer = new Singer();
		std::cout << "Singer name:        ";
		readChars(singer->name, sizeof(singer->name));
		if (std::string{ singer->name } == "/STOP")
			break;
		std::cout << "Years (xxxx-yyyy):  ";
		readChars(singer->years, sizeof(singer->years));
		std::cout << "Country:       ";
		readChars(singer->country, sizeof(singer->country));
		std::cout << '\n';
		singer->id = singer->getNewID();
		Singers.push_back(singer);
	}
}

// Transformation from sec to xx:yy:zz format
std::string sec2hms(unsigned time) {
	std::string hms;
	unsigned hint = time / 3600;
	unsigned mint = time % 3600 / 60;
	unsigned sint = time % 60;
	std::string hstr;
	std::string mstr;
	std::string sstr;
	hstr = std::to_string(hint);
	mstr = std::to_string(mint);
	sstr = std::to_string(sint);
	if (hint < 10) {
		hstr = "0" + hstr;
	}
	if (mint < 10) {
		mstr = "0" + mstr;
	}
	if (sint < 10) {
		sstr = "0" + sstr;
	}
	hms = hstr + ":" + mstr + ":" + sstr;
	return hms;
}

// Finding album title from album ID
std::string findAlbumTitle(unsigned album_id) {
	std::string album_title = "";
	for (const auto& album : Albums) {
		if (album->id == album_id) {
			album_title = album->title;
		}
	}
	if (album_title == "") {
		album_title = "Error: undefined album!";
	}
	return album_title;
}

// Finding name of the singer from singer ID
std::string findSingerName(unsigned singer_id) {
	std::string singer_name = "";
	for (const auto& singer : Singers) {
		if (singer->id == singer_id) {
			singer_name = singer->name;
		}
	}
	if (singer_name == "") {
		singer_name = "Error: undefined singer!";
	}
	return singer_name;
}

// Finding ID of the singer from its album ID
int findSingerIDFromAlbum(unsigned album_id) {
	int singer_id = 0;
	for (auto& album : Albums) {
		if (album->id == album_id) {
			singer_id = album->singer_id;
		}
	}
	return singer_id;
}

// Finding IDs of all albums from singer ID
int findAlbumsFromSingers(unsigned singer_id) {
	foundAlbums.clear();
	for (const auto& album : Albums) {
		if (singer_id == 0) {
			for (unsigned i = 0; i < foundSingers.size(); i++) {
				if (album->singer_id == foundSingers[i])
					foundAlbums.push_back(album->id);
			}
		}
		else {
			if (album->singer_id == singer_id) {
				foundAlbums.push_back(album->id);
			}
		}
	}
	return 0;
}

// Finding IDs of all songs from album ID
int findSongsFromAlbums(unsigned album_id) {
	foundSongs.clear();
	for (const auto& song : Songs) {
		if (album_id == 0) {
			for (unsigned i = 0; i < foundAlbums.size(); i++) {
				if (song->album_id == foundAlbums[i])
					foundSongs.push_back(song->id);
			}
		}
		else {
			if (song->album_id == album_id) {
				foundSongs.push_back(song->id);
			}
		}
	}
	return 0;
}

// Finding IDs of all singers from country
int findSingersFromCountry(std::string country) {
	foundSingers.clear();
	for (const auto& singer : Singers) {
		if (singer->country == country) {
			foundSingers.push_back(singer->id);
		}
	}
	return 0;
}

// Getting an album duration from sum of all songs' duration
unsigned getSongsDuration(unsigned album_id) {
	unsigned alltime = 0;
	for (const auto& song : Songs) {
		if (song->album_id == album_id) {
			alltime += song->duration;
		}
	}
	return alltime;
}

// Deleting all data of song
void clearSong(unsigned index) {
	delete Songs[index];
	Songs.erase(Songs.begin() + index);
}

// Deleting all or part of songs
void clearSongs(unsigned alb_id) {
	if (alb_id == 0) {
		for (unsigned j = 0; j < Songs.size();) {
			clearSong(j);
		}
	}
	else {
		for (unsigned j = 0; j < Songs.size();) {
			if (Songs[j]->album_id == alb_id) {
				clearSong(j);
			}
			else {
				j++;
			}
		}
	}
}

// Deleting all data of album
void clearAlbum(unsigned index) {
	delete Albums[index];
	Albums.erase(Albums.begin() + index);
}

// Deleting all or part of albums
void clearAlbums(unsigned singer_id) {
	if (singer_id == 0) {
		for (unsigned i = 0; i < Albums.size();) {
			clearAlbum(i);
		}
	}
	else {
		for (unsigned i = 0; i < Albums.size();) {
			if (Albums[i]->singer_id == singer_id) {
				clearSongs(Albums[i]->id);
				clearAlbum(i);
			}
			else {
				i++;
			}
		}
	}
}

// Deleting all data of singer
void clearSinger(unsigned index) {
	delete Singers[index];
	Singers.erase(Singers.begin() + index);
}

// Deleting all of singers
void clearSingers(unsigned x) {
	if (x == 0) {
		for (unsigned i = 0; i < Singers.size();) {
			clearSinger(i);
		}
	}
}

// Finding filtered singers
void filterSingers(void) {
	foundSingers.clear();
	std::string tag = "";
	std::cout << "By which tag you want to filter elements?\n\n";
	std::cout << "To filter elements by:\n";
	std::cout << "Country enter 'country'\n";
	std::cin >> tag;
	if (tag == "country") {			// Filtering by country
		std::string country;
		std::cout << "\nEnter the country:			";
		std::cin >> country;
		findSingersFromCountry(country);
		std::cout << "\nList of the filtered elements by country " + country + ":\n";
	}
	else {
		std::cout << "Please enter valid tag!\n\n";
		filterSingers();
	}
}

// Finding filtered albums
void filterAlbums(void) {
	foundAlbums.clear();
	std::string tag = "";
	std::cout << "By which tag you want to filter elements?\n\n";
	std::cout << "To filter elements by:\n";
	std::cout << "Singer ID enter 'singerID'\n";
	std::cout << "Country enter 'country'\n";
	std::cout << "Year of release enter 'year'\n";
	std::cin >> tag;
	if (tag == "singerID") {		// Filtering by singer ID
		int singerID;
		std::cout << "\nEnter the singer ID:		";
		std::cin >> singerID;
		findAlbumsFromSingers(singerID);
		std::cout << "\nList of the filtered elements by singer ID " + std::to_string(singerID) + ":\n";
	}
	else if (tag == "country") {	// Filtering by country
		std::string country;
		std::cout << "\nEnter the genre:			";
		std::cin >> country;
		findSingersFromCountry(country);
		findAlbumsFromSingers(0);
		foundSingers.clear();
		std::cout << "\nList of the filtered elements by country " + country + ":\n";
	}
	else if (tag == "year") {		// Filtering by year of release
		unsigned year;
		std::cout << "\nEnter the year of release:	";
		std::cin >> year;
		for (const auto& album : Albums) {
			if (album->year_of_release == year)
				foundAlbums.push_back(album->id);
		}
		std::cout << "\nList of the filtered elements by year " + std::to_string(year) + ":\n";
	}
	else {
		std::cout << "Please enter valid tag!\n\n";
		filterAlbums();
	}
}

// Finding of filtered songs
void filterSongs(void) {
	foundSongs.clear();
	std::string tag = "";
	std::cout << "By which tag you want to filter elements?\n\n";
	std::cout << "To filter elements by:\n";
	std::cout << "Singer ID enter 'singerID'\n";
	std::cout << "Album ID enter 'albumID'\n";
	std::cout << "Genre enter 'genre'\n";
	std::cout << "Audiocodec enter 'audiocodec'\n";
	std::cin >> tag;
	if (tag == "audiocodec") {		// Filtering by audiocodec
		std::string audiocodec;
		std::cout << "\nEnter the audiocodec:		";
		std::cin >> audiocodec;
		for (const auto& song : Songs) {
			if (song->audiocodec == audiocodec)
				foundSongs.push_back(song->id);
		}
		std::cout << "\nList of the filtered elements by audiocodec " + audiocodec + ":\n";
	}
	else if (tag == "genre") {		// Filtering by genre
		std::string genre;
		std::cout << "\nEnter the genre:			";
		std::cin >> genre;
		for (const auto& song : Songs) {
			if (song->genre == genre)
				foundSongs.push_back(song->id);
		}
		std::cout << "\nList of the filtered elements by genre " + genre + ":\n";
	}
	else if (tag == "albumID") {	// Filtering by album ID
		unsigned albumID;
		std::cout << "\nEnter the album ID:			";
		albumID = getInteger();
		for (const auto& song : Songs) {
			if (song->album_id == albumID)
				foundSongs.push_back(song->id);
		}
		std::cout << "\nList of the filtered elements by album ID " + std::to_string(albumID) + ":\n";
	}
	else if (tag == "singerID") {	// Filtering by singer ID
		unsigned singerID;
		std::cout << "\nEnter the singer ID:		";
		singerID = getInteger();
		findAlbumsFromSingers(singerID);
		for (const auto& song : Songs) {
			for (unsigned i = 0; i < foundAlbums.size(); i++) {
				if (song->album_id == foundAlbums[i])
					foundSongs.push_back(song->id);
			}
		}
		foundAlbums.clear();
		std::cout << "\nList of the filtered elements by singer ID " + std::to_string(singerID) + ":\n";
	}
	else {
		std::cout << "Please enter valid data!\n\n";
		filterSongs();
	}
}

// Loading data about singers, albums and songs from binary files
void loadFromFile() {
	clearSongs(0);
	clearAlbums(0);
	clearSingers(0);
	if (std::ifstream ifs{ Singers_fname, std::ios::binary | std::ios::ate }) {
		unsigned fileSize = (unsigned)ifs.tellg();
		ifs.seekg(0);
		while (ifs.tellg() < fileSize) {
			Singer* singer = new Singer();
			ifs.read((char*)singer, sizeof(Singer));
			Singers.push_back(singer);
		}
	}
	if (std::ifstream ifs{ Albums_fname, std::ios::binary | std::ios::ate }) {
		unsigned int fileSize = (unsigned)ifs.tellg();
		ifs.seekg(0);
		while (ifs.tellg() < fileSize) {
			Album* album = new Album();
			ifs.read((char*)album, sizeof(Album));
			Albums.push_back(album);
		}
	}
	if (std::ifstream ifs{ Songs_fname, std::ios::binary | std::ios::ate }) {
		unsigned int fileSize = (unsigned)ifs.tellg();
		ifs.seekg(0);
		while (ifs.tellg() < fileSize) {
			Song* song = new Song();
			ifs.read((char*)song, sizeof(Song));
			Songs.push_back(song);
		}
	}
}

// Saving data about singers, albums and songs into binary files
void saveToFile() {
	if (std::ofstream ofs{ Singers_fname, std::ios::binary | std::ios::trunc }) {
		for (auto& singer : Singers) {
			char* data = reinterpret_cast<char*>(singer);
			ofs.write(data, sizeof(*singer));
		}
	}
	if (std::ofstream ofs{ Albums_fname, std::ios::binary | std::ios::trunc }) {
		for (auto& album : Albums) {
			char* data = reinterpret_cast<char*>(album);
			ofs.write(data, sizeof(*album));
		}
	}
	if (std::ofstream ofs{ Songs_fname, std::ios::binary | std::ios::trunc }) {
		for (auto& song : Songs) {
			char* data = reinterpret_cast<char*>(song);
			ofs.write(data, sizeof(*song));
		}
	}
}

// Showing detailed information about singer
void selectSinger() {
	unsigned singID;
	std::cout << "\nEnter ID of the singer which information you want to see:		";
	singID = getInteger();
	bool b = 0;
	for (auto& singer : Singers) {
		if (singer->id == singID) {
			std::cout << "\nSinger name: " << singer->name << ";";
			std::cout << "\nCountry: " << singer->country << ";";
			std::cout << "\nYears of activity: " << singer->years << ";";
			std::cout << "\nCount of albums: ";
			findAlbumsFromSingers(singID);
			std::cout << foundAlbums.size() << ";";
			std::cout << "\nCount of songs: ";
			findAlbumsFromSingers(singID);
			findSongsFromAlbums(0);
			std::cout << foundSongs.size() << ";\n";
			b = 1;
		}
	}
	if (b == 0) {
		std::cout << "\nSorry, singer with ID " << singID << " couldn't be found!\n";
	}
}

// Showing detailed information about album
void selectAlbum() {
	unsigned albID;
	std::cout << "\nEnter ID of the album which information you want to see:		";
	albID = getInteger();
	bool b = 0;
	for (auto& album : Albums) {
		if (album->id == albID) {
			std::cout << "\nAlbum title: " << album->title << ";";
			std::cout << "\nSinger: " << findSingerName(album->singer_id) << ";";
			std::cout << "\nYear of release: " << album->year_of_release << ";";
			std::cout << "\nCount of songs: ";
			findSongsFromAlbums(albID);
			std::cout << foundSongs.size() << ";";
			std::cout << "\nAlbum duration: ";
			std::cout << sec2hms(getSongsDuration(album->id)) << ";\n";
			b = 1;
		}
	}
	if (b == 0) {
		std::cout << "\nSorry, album with ID " << albID << " couldn't be found!\n";
	}
}

// Showing detailed information about song
void selectSong() {
	unsigned songID;
	std::cout << "\nEnter ID of the song which information you want to see:		";
	songID = getInteger();
	bool b = 0;
	for (auto& song : Songs) {
		if (song->id == songID) {
			std::cout << "\nSong title: " << song->title << ";";
			std::cout << "\nAlbum: " << findAlbumTitle(song->album_id) << ";";
			std::cout << "\nSinger: " << 
				findSingerName(findSingerIDFromAlbum(song->album_id)) 
				<< ";";
			std::cout << "\nGenre: " << song->genre << ";";
			std::cout << "\nDuration: " << sec2hms(song->duration) << ";";
			std::cout << "\nAudiocodec: " << song->audiocodec << ";";
			std::cout << "\nCount of listenings: " << song->count_of_listens << ";\n";
			b = 1;
		}
	}
	if (b == 0) {
		std::cout << "\nSorry, song with ID " << songID << " couldn't be found!\n";
	}
}

// Sorting all singers
void sortSingers() {
	foundSingers.clear();
	std::string tag;
	std::cout << "\nBy which tag you want to sort singers?\n\n";
	std::cout << "To sort singers by:\n";
	std::cout << "Name enter 'name'\n";
	std::cin >> tag;
	if (tag == "name") {
		while (foundSingers.size() != Singers.size()) {
			unsigned maxID = 0;
			char maxName[32] = {};
			for (unsigned i = 0; i < Singers.size(); i++) {
				bool k = 0;
				for (unsigned l = 0; l < sizeof(Singers[i]->name);) {
					if (tolower(Singers[i]->name[l]) > tolower(maxName[l])) {
						for (unsigned j = 0; j < foundSingers.size(); j++) {
							if (Singers[i]->id == foundSingers[j]) {
								k = 1;
							}
						}
						break;
					}
					else if (tolower(Singers[i]->name[l]) == tolower(maxName[l])) {
						l++;
					}
					else {
						k = 1;
						break;
					}
				}
				if (k == 0) {
					maxID = Singers[i]->id;
					for (unsigned m = 0; m < sizeof(Singers[i]->name); m++) {
						maxName[m] = Singers[i]->name[m];
					}
				}
			}
			foundSingers.push_back(maxID);
		}
	}
	else {
		std::cout << "\nYou've entered invalid data! Try again!\n";
		sortSingers();
	}
}

// Sorting all albums 
void sortAlbums() {
	foundAlbums.clear();
	std::string tag;
	std::cout << "\nBy which tag you want to sort albums?\n\n";
	std::cout << "To sort albums by:\n";
	std::cout << "Title enter 'title'\n";
	std::cout << "Duration enter 'duration'\n";
	std::cout << "Year of release enter 'year'\n\n";
	std::cin >> tag;
	if (tag == "duration" || tag == "title" || tag == "year") {
		while (foundAlbums.size() != Albums.size()) {
			unsigned maxID = 0;
			unsigned maxDur = 0;
			unsigned maxYear = 0;
			char maxTitle[32] = {};
			for (unsigned i = 0; i < Albums.size(); i++) {
				bool k = 0;
				for (unsigned l = 0; l < sizeof(Albums[i]->title);) {
					if ((tag == "duration" && getSongsDuration(Albums[i]->id) >= maxDur) ||
						(tag == "year" && Albums[i]->year_of_release >= maxYear) ||
						(tag == "title" && tolower(Albums[i]->title[l]) > tolower(maxTitle[l]))) {
						for (unsigned j = 0; j < foundAlbums.size(); j++) {
							if (Albums[i]->id == foundAlbums[j]) {
								k = 1;
							}
						}
						break;
					}
					else if (tag == "title" && tolower(Albums[i]->title[l]) == tolower(maxTitle[l])) {
						l++;
					}
					else {
						k = 1;
						break;
					}
				}
				if (k == 0) {
					maxID = Albums[i]->id;
					maxDur = getSongsDuration(Albums[i]->id);
					maxYear = Albums[i]->year_of_release;
					for (unsigned m = 0; m < sizeof(Albums[i]->title); m++) {
						maxTitle[m] = Albums[i]->title[m];
					}
				}
			}
			foundAlbums.push_back(maxID);
		}
	}
	else {
		std::cout << "\nYou've entered invalid data! Try again!\n";
		sortAlbums();
	}
}

// Sorting all songs
void sortSongs() {
	foundSongs.clear();
	std::string tag;
	std::cout << "\nBy which tag you want to sort songs?\n\n";
	std::cout << "To sort songs by:\n";
	std::cout << "Title enter 'title'\n";
	std::cout << "Duration enter 'duration'\n";
	std::cout << "Count of listenings enter 'count'\n\n";
	std::cin >> tag;
	if (tag == "duration" || tag == "title" || tag == "count") {
		while (foundSongs.size() != Songs.size()) {
			unsigned maxID = 0;
			unsigned maxDur = 0;
			unsigned maxCount = 0;
			char maxTitle[32] = {};
			for (unsigned i = 0; i < Songs.size(); i++) {
				bool k = 0;
				for (unsigned l = 0; l < sizeof(Songs[i]->title);) {
					if ((tag == "duration" && Songs[i]->duration >= maxDur) ||
						(tag == "count" && Songs[i]->count_of_listens >= maxCount) ||
						(tag == "title" && tolower(Songs[i]->title[l]) > tolower(maxTitle[l]))) {
						for (unsigned j = 0; j < foundSongs.size(); j++) {
							if (Songs[i]->id == foundSongs[j]) {
								k = 1;
							}
						}
						break;
					}
					else if (tag == "title" && tolower(Songs[i]->title[l]) == tolower(maxTitle[l])) {
						l++;
					}
					else {
						k = 1;
						break;
					}
				}
				if (k == 0) {
					maxID = Songs[i]->id;
					maxDur = Songs[i]->duration;
					maxCount = Songs[i]->count_of_listens;
					for (unsigned m = 0; m < sizeof(Songs[i]->title); m++) {
						maxTitle[m] = Songs[i]->title[m];
					}
				}
			}
			foundSongs.push_back(maxID);
		}
	}
	else {
	std::cout << "\nYou've entered invalid data! Try again!\n";
	sortSongs();
	}
}

// Deleting selected songs
void deleteSongs (void) {
	std::cout << "When you done, enter 0" << "\n";
	unsigned song_id = 1;
	bool k;
	while (song_id != 0) {
		std::cout << "Enter ID of the song that you'd like to delete: " << "\n";
		song_id = getInteger();
		k = 0;
		for (unsigned i = 0; i < Songs.size(); i++) {
			if (Songs[i]->id == song_id) {
				std::cout << "Song " << Songs[i]->title << " has been deleted.\n";
				clearSong(i);
				k = 1;
			}
		}
		if (k == 0 && song_id != 0) {
			std::cout << "There isn't exist song with ID " << song_id << "!\n";
		}
	}
}

// Deleting selected albums
void deleteAlbums(void) {
	std::cout << "When you done, enter 0" << "\n";
	unsigned album_id = 1;
	bool k;
	while (album_id != 0) {
		std::cout << "Enter ID of the album that you'd like to delete: " << "\n";
		album_id = getInteger();
		k = 0;
		for (unsigned i = 0; i < Albums.size(); i++) {
			if (Albums[i]->id == album_id) {
				clearSongs(Albums[i]->id);
				std::cout << "Album " << Albums[i]->title << " has been deleted.\n";
				clearAlbum(i);
				k = 1;
			}
		}
		if (k == 0 && album_id != 0) {
			std::cout << "There isn't exist album with ID " << album_id << "!\n";
		}
	}
}

// Deleting selected singers
void deleteSingers(void) {
	std::cout << "When you done, enter 0" << "\n";
	unsigned singer_id = 1;
	bool k;
	while (singer_id != 0) {
		std::cout << "Enter ID of the singer that you'd like to delete: " << "\n";
		singer_id = getInteger();
		k = 0;
		for (unsigned i = 0; i < Singers.size(); i++) {
			if (Singers[i]->id == singer_id) {
				clearAlbums(Singers[i]->id);
				std::cout << "Singer " << Singers[i]->name << " has been deleted.\n";
				clearSinger(i);
				k = 1;
			}
		}
		if (k == 0 && singer_id != 0) {
			std::cout << "There isn't exist singer with ID " << singer_id << "!\n";
		}
	}
}

// Showing an information about singers
std::ostream& operator<<(std::ostream& os, const Singer* singer) {
	return os << std::setfill(' ') << "| " << std::right
		<< std::setw(5)
		<< singer->id
		<< " | " << std::left
		<< std::setw(32)
		<< singer->name
		<< " |";
}

// Showing an information about albums
std::ostream& operator<<(std::ostream& os, const Album* album) {
	return os << std::setfill(' ') << "| " << std::right
		<< std::setw(5)
		<< album->id
		<< " | " << std::left
		<< std::setw(32)
		<< album->title
		<< " | " << std::left
		<< std::setw(32)
		<< findSingerName(album->singer_id)
		<< " |";
}

// Showing an information about songs
std::ostream& operator<<(std::ostream& os, const Song* song) {
	return os << std::setfill(' ') << "| " << std::right
		<< std::setw(5)
		<< song->id
		<< " | " << std::left
		<< std::setw(32)
		<< song->title
		<< " | " << std::left
		<< std::setw(32)
		<< findAlbumTitle(song->album_id)
		<< " |";
}

// /menu command
void showMenu() {
	std::cout << std::setfill('-') << std::setw(70) << "\n";
	std::cout << "\n| " << std::setfill(' ') << std::setw(10)
		<< std::left << "/create" << " | " << std::setw(52)
		<< "Create new objects" << " |";
	std::cout << std::setfill('-') << std::setw(70) << "\n";
	std::cout << "\n| " << std::setfill(' ') << std::setw(10)
		<< std::left << "/load" << " | " << std::setw(52)
		<< "Load objects from binary files" << " |";
	std::cout << std::setfill('-') << std::setw(70) << "\n";
	std::cout << "\n| " << std::setfill(' ') << std::setw(10)
		<< std::left << "/view" << " | " << std::setw(52)
		<< "Show all created and loaded objects" << " |";
	std::cout << std::setfill('-') << std::setw(70) << "\n";
	std::cout << "\n| " << std::setfill(' ') << std::setw(10)
		<< std::left << "/select" << " | " << std::setw(52)
		<< "See detailed information about object" << " |";
	std::cout << std::setfill('-') << std::setw(70) << "\n";
	std::cout << "\n| " << std::setfill(' ') << std::setw(10)
		<< std::left << "/filter" << " | " << std::setw(52)
		<< "Display filtered objects" << " |";
	std::cout << std::setfill('-') << std::setw(70) << "\n";
	std::cout << "\n| " << std::setfill(' ') << std::setw(10)
		<< std::left << "/sort" << " | " << std::setw(52)
		<< "Sort objects by some criterias" << " |";
	std::cout << std::setfill('-') << std::setw(70) << "\n";
	std::cout << "\n| " << std::setfill(' ') << std::setw(10)
		<< std::left << "/save" << " | " << std::setw(52)
		<< "Save existing objects into binary files" << " |";
	std::cout << std::setfill('-') << std::setw(70) << "\n";
	std::cout << "\n| " << std::setfill(' ') << std::setw(10)
		<< std::left << "/delete" << " | " << std::setw(52)
		<< "Delete objects from database" << " |";
	std::cout << std::setfill('-') << std::setw(70) << "\n";
	std::cout << "\n| " << std::setfill(' ') << std::setw(10)
		<< std::left << "/exit" << " | " << std::setw(52)
		<< "Close the program" << " |";
	std::cout << std::setfill('-') << std::setw(70) << "\n" << "\n";
}

int main() {
	std::string command;
	while (true) {
		std::cout << "\nTo show menu enter '/menu' command\n";
		std::cin >> command;
		if (command == "/menu") {
			showMenu();
		}
		else if (command == "/create") {
			int whatToCreate;
			std::cout << "\nWhat would you like to create?\n\n";
			std::cout << "To create new singer press 1\n";
			if (Singers.size() != 0) {
				std::cout << "To create new album press 2\n";
				if (Albums.size() != 0) {
					std::cout << "To create new song press 3\n";
				}
			}
			whatToCreate = getInteger();
			if (whatToCreate == 1) {
				createSingers();
			}
			else if (whatToCreate == 2 && Singers.size() != 0) {
				createAlbums();
			}
			else if (whatToCreate == 3 && Albums.size() != 0) {
				createSongs();
			}
			else {
				std::cout << "You've entered invalid data!\n";
			}
		}
		else if (command == "/view") {
			bool k = 0;
			if (Singers.size() != 0) {
				std::cout << "\nList of all singers:\n\n";
				for (const auto& singer : Singers) {
					std::cout << singer << '\n';		// Listing of all singers
				}
				k = 1;
			}
			if (Albums.size() != 0) {
				std::cout << "\nList of all albums:\n\n";
				for (const auto& album : Albums) {
					std::cout << album << '\n';			// Listing of all albums
				}
				k = 1;
			}
			if (Songs.size() != 0) {
				std::cout << "\nList of all songs:\n\n";
				for (const auto& song : Songs) {
					std::cout << song << '\n';			// Listing of all songs
				}
				k = 1;
			}
			if (k == 0) {
				std::cout << "\nYou don't have any data!\n";
			}
		}
		else if (command == "/select") {
			int whatToSelect;
			if (Singers.size() != 0) {
				std::cout << "\nWhat would you like to select?\n\n";
				std::cout << "To view detailed information about singer press 1\n";
				if (Albums.size() != 0) {
					std::cout << "To view detailed information about album press 2\n";
					if (Songs.size() != 0) {
						std::cout << "To view detailed information about song press 3\n";
					}
				}
				whatToSelect = getInteger();
				if (whatToSelect == 1 && Singers.size() != 0) {
					for (const auto& singer : Singers) {
						std::cout << singer << '\n';
					}
					selectSinger();
				}
				else if (whatToSelect == 2 && Albums.size() != 0) {
					for (const auto& album : Albums) {
						std::cout << album << '\n';
					}
					selectAlbum();
				}
				else if (whatToSelect == 3 && Songs.size() != 0) {
					for (const auto& song : Songs) {
						std::cout << song << '\n';
					}
					selectSong();
				}
				else {
					std::cout << "You've entered invalid data!\n";
				}
			}
			else {
				std::cout << "You don't have any data!\n";
			}
		}
		else if (command == "/filter") {
			if (Singers.size() != 0) {
				int whatToFilter;
				std::cout << "\nWhat would you like to filter?\n\n";
				std::cout << "To filter singers press 1\n";
				if (Albums.size() != 0) {
					std::cout << "To filter albums press 2\n";
					if (Songs.size() != 0) {
						std::cout << "To filter songs press 3\n\n";
					}
				}
				whatToFilter = getInteger();
				if (whatToFilter == 1 ||
					(whatToFilter == 2 && Albums.size() != 0) ||
					(whatToFilter == 3 && Songs.size() != 0)) {
					char choose;
					std::cout << "if you want to see all elements excluding filtered enter -\n";
					std::cout << "if you want to see only filtered elements enter +\n";
					std::cin >> choose;		// Which elements will we see?
					if (choose == '+' || choose == '-') {
						bool k = 0;
						if (whatToFilter == 1) {
							filterSingers();
							for (const auto& singer : Singers) {
								bool j = 0;
								for (unsigned i = 0; i < foundSingers.size(); i++) {
									if (singer->id == foundSingers[i]) {
										j = 1;
									}
								}
								if ((choose == '-' && j == 0) || (choose == '+' && j != 0)) {
									std::cout << singer << '\n';
									k = 1;
								}
							}
						}
						else if (whatToFilter == 2) {
							filterAlbums();
							for (const auto& album : Albums) {
								bool j = 0;
								for (unsigned i = 0; i < foundAlbums.size(); i++) {
									if (album->id == foundAlbums[i]) {
										j = 1;
									}
								}
								if ((choose == '-' && j == 0) || (choose == '+' && j != 0)) {
									std::cout << album << '\n';
									k = 1;
								}
							}
						}
						else {
							filterSongs();
							for (const auto& song : Songs) {
								bool j = 0;
								for (unsigned i = 0; i < foundSongs.size(); i++) {
									if (song->id == foundSongs[i]) {
										j = 1;
									}
								}
								if ((choose == '-' && j == 0) || (choose == '+' && j != 0)) {
									std::cout << song << '\n';
									k = 1;
								}
							}
						}
						if (k == 0) {
							std::cout << "Elements that match the filtering condition were not found!\n";
						}
					}
					else {
						std::cout << "Please enter '-' or '+' sign next time!\n";
					}
				}
				else {
					std::cout << "You've entered invalid data!\n";
				}
			}
			else {
				std::cout << "There's nothing to filter!\n";
			}
		}
		else if (command == "/sort") {
			if (Singers.size() != 0) {
				int whatToSort;
				std::cout << "\nWhat would you like to sort?\n\n";
				std::cout << "To sort singers press 1\n";
				if (Albums.size() != 0) {
					std::cout << "To sort albums press 2\n";
					if (Songs.size() != 0) {
						std::cout << "To sort songs press 3\n";
					}
				}
				whatToSort = getInteger();
				if (whatToSort == 1 ||
					(whatToSort == 2 && Albums.size() != 0) ||
					(whatToSort == 3 && Songs.size() != 0)) {
					char choose;
					std::cout << "\nif you want to sort elements in ascending order enter >\n";
					std::cout << "if you want to sort elements in descending order enter <\n";
					std::cin >> choose;		// In what order we will see sorted elements?
					if (choose == '<' || choose == '>') {
						if (whatToSort == 1) {
							sortSingers();
							for (unsigned i = 0; i < foundSingers.size(); i++) {
								for (const auto& singer : Singers) {
									if ((choose == '<' && singer->id == foundSingers[i]) ||
										(choose == '>' && singer->id == foundSingers[(foundSingers.size() - 1) - i])) {
										std::cout << singer << '\n';
									}
								}
							}
						}
						else if (whatToSort == 2) {
							sortAlbums();
							for (unsigned i = 0; i < foundAlbums.size(); i++) {
								for (const auto& album : Albums) {
									if ((choose == '<' && album->id == foundAlbums[i]) ||
										(choose == '>' && album->id == foundAlbums[(foundAlbums.size() - 1) - i])) {
										std::cout << album << '\n';
									}
								}
							}
						}
						else {
							sortSongs();
							for (unsigned i = 0; i < foundSongs.size(); i++) {
								for (const auto& song : Songs) {
									if ((choose == '<' && song->id == foundSongs[i]) ||
										(choose == '>' && song->id == foundSongs[(foundSongs.size() - 1) - i])) {
										std::cout << song << '\n';
									}
								}
							}
						}
					}
					else {
						std::cout << "Please enter '<' or '>' sign!\n";
					}
				}
				else {
					std::cout << "You've entered invalid data!\n";
				}
			}
			else {
				std::cout << "There's nothing to sort!\n";
			}
		}
		else if (command == "/save") {
			saveToFile();
			std::cout << "\nSaving is completed!\n\n";
		}
		else if (command == "/load") {
			loadFromFile();
			std::cout << "\nLoading is completed!\n\n";
		}
		else if (command == "/delete") {
			if (Singers.size() != 0) {
				int whatToDelete;
				std::cout << "\nWhat would you like to delete?\n\n";
				std::cout << "To delete singer/s press 1\n";
				if (Albums.size() != 0) {
					std::cout << "To delete album/s press 2\n";
					if (Songs.size() != 0) {
						std::cout << "To delete song/s press 3\n";
					}
				}
				std::cout << "\nTo delete all data enter 0\n";
				whatToDelete = getInteger();
				if (whatToDelete == 1) {
					deleteSingers();
				}
				else if (whatToDelete == 2 && Singers.size() != 0) {
					deleteAlbums();
				}
				else if (whatToDelete == 3 && Albums.size() != 0) {
					deleteSongs();
				}
				else if (whatToDelete == 0) {
					std::string areyousure;
					std::cout << "\nAre you sure you want to clear all data?\n";
					std::cout << "(Enter TRUE if yes)\n";
					std::cin >> areyousure;
					if (areyousure == "TRUE") {
						clearSongs(0);
						clearAlbums(0);
						clearSingers(0);
						std::cout << "All data has been cleared.\n";
					}
					else {
						std::cout << "Deleting of all data has been cancelled.\n";
					}
				}
				else {
					std::cout << "You've entered invalid data!\n";
				}
			}
			else {
				std::cout << "There's nothing to delete!\n";
			}
		}
		else if (command == "/exit") {
			return 0;
		}
	}
}