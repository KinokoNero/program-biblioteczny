#include <windows.h>
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

class DataCounter {
public:
    int getDataAmount(short DATA) {return HowManyData(DATA);}

private:
    int LinesAmount;
    int DataAmount;
    string dump;

    int HowManyData(short DATA) {
        LinesAmount = 0;
        DataAmount = 0;
        if(DATA == 0) {
            ifstream Books;
            Books.open("Books.bin", ios::binary);
            if(Books.is_open() && !Books.eof()) {
                while(getline(Books, dump))
                    LinesAmount++;
            }
            if(Books.is_open()) Books.close();
            DataAmount = LinesAmount/5;
        }
        else if(DATA == 1) {
            ifstream Readers;
            Readers.open("Readers.bin", ios::binary);
            if(Readers.is_open() && !Readers.eof()) {
                while(getline(Readers, dump))
                    LinesAmount++;
            }
            if(Readers.is_open()) Readers.close();
            DataAmount = LinesAmount/6;
        }
        else if(DATA == 2) {
            ifstream Borrows;
            Borrows.open("Borrows.bin", ios::binary);
            if(Borrows.is_open() && !Borrows.eof()) {
                while(getline(Borrows, dump))
                    LinesAmount++;
            }
            if(Borrows.is_open()) Borrows.close();
            DataAmount = LinesAmount/5;
        }

        return DataAmount;
    }
};

class DataPicker {
public:
    string status, id, title, author, date;
    string pesel, name, phone, por; // por = place of residence
    string borrow_reader_id, borrow_book_id, borrow_date;

    void RetrieveData(short DATA, int id) {ChooseData(DATA, id);}

private:
    string dump;

    void ChooseData(short DATA, int id) {
        status = this->id = title = author = date = pesel = name = phone = por = borrow_reader_id = borrow_book_id = borrow_date = "";
        if(DATA == 0) {
            ifstream Books;
            Books.open("Books.bin", ios::binary);
            if(Books.is_open() && !Books.eof()) {
                for(int i=0; i<(id*5); i++) {
                        getline(Books, dump);
                }
                getline(Books, status);
                getline(Books, this->id);
                getline(Books, title);
                getline(Books, author);
                getline(Books, date);
            }
            if(Books.is_open()) Books.close();
        }
        else if(DATA == 1) {
            ifstream Readers;
            Readers.open("Readers.bin", ios::binary);
            if(Readers.is_open() && !Readers.eof()) {
                for(int i=0; i<(id*6); i++) {
                        getline(Readers, dump);
                }
                getline(Readers, status);
                getline(Readers, this->id);
                getline(Readers, pesel);
                getline(Readers, name);
                getline(Readers, phone);
                getline(Readers, por);
            }
            if(Readers.is_open()) Readers.close();
        }
        else if(DATA == 2) {
            ifstream Borrows;
            Borrows.open("Borrows.bin", ios::binary);
            if(Borrows.is_open() && !Borrows.eof()) {
                for(int i=0; i<(id*5); i++) {
                        getline(Borrows, dump);
                }
                getline(Borrows, status);
                getline(Borrows, this->id);
                getline(Borrows, borrow_reader_id);
                getline(Borrows, borrow_book_id);
                getline(Borrows, borrow_date);
            }
            if(Borrows.is_open()) Borrows.close();
        }
    }
};

class DataMaster : public DataCounter, public DataPicker {
public:
    void setAddData(short DATA, string str_data0, string str_data1, short sho_data0, long long ll_data0, long long ll_data1) {AddData(DATA, str_data0, str_data1, sho_data0, ll_data0, ll_data1);}
    void setDataStatus(short DATA, string id, string status) {DataStatus(DATA, id, status);}
    string getListData(short DATA, string status) {return ListData(DATA, status);}
    string getSearchData(short DATA, string filter, string status) {return SearchData(DATA, filter, status);}

    bool Error = false;
    bool Change = false;

private:
//----------------------------------------------------------------------------------------------------------------------------------------------Add Data
    void AddData(short DATA, string str_data0, string str_data1, short sho_data0, long long ll_data0, long long ll_data1) {
        Error = false;
        if(DATA == 0) {
            string title = str_data0;
            string author = str_data1;
            short date = sho_data0;
            ofstream Books;
            Books.open("Books.bin", ios::app | ios::binary);
            if(Books.is_open()) {
                Books << "ACTIVE\n";
                Books << getDataAmount(DATA) << "\n";
                Books << title << "\n";
                Books << author << "\n";
                Books << date << "\n";
            }
            if(Books.is_open()) Books.close();
        }
        else if(DATA == 1) {
            long long pesel = ll_data0;
            string name = str_data0;
            long long phone = ll_data1;
            string por = str_data1;
            ofstream Readers;
            Readers.open("Readers.bin", ios::app | ios::binary);
            if(Readers.is_open()) {
                Readers << "ACTIVE\n";
                Readers << getDataAmount(DATA) << "\n";
                Readers << pesel << "\n";
                Readers << name << "\n";
                Readers << phone << "\n";
                Readers << por << "\n";
            }
            if(Readers.is_open()) Readers.close();
        }
        else if(DATA == 2) {
            try {
                string borrow_reader_id = str_data0;
                string borrow_book_id = str_data1;

                int int_reader_id = stoi(borrow_reader_id);
                int int_book_id = stoi(borrow_book_id);

                RetrieveData(1, int_reader_id);
                if(id != borrow_reader_id || status != "ACTIVE") {
                    system("cls");
                    cout << "Wybrany czytelnik nie jest dostepny!";
                    Sleep(2000);
                    return;
                }
                RetrieveData(0, int_book_id);
                if(id != borrow_book_id || status != "ACTIVE") {
                    system("cls");
                    cout << "Wybrana ksiazka nie jest dostepna!";
                    Sleep(2000);
                    return;
                }
                time_t t = time(0);
                tm * now = localtime(&t);

                DataStatus(0, borrow_book_id, "BORROWED");
                ofstream Borrows;
                Borrows.open("Borrows.bin", ios::app | ios::binary);
                if(Borrows.is_open()) {
                    Borrows << "ACTIVE\n";
                    Borrows << getDataAmount(DATA) << "\n";
                    Borrows << borrow_reader_id << "\n";
                    Borrows << borrow_book_id << "\n";
                    Borrows << now->tm_mday << "-" << now->tm_mon+1 << "-" << now->tm_year+1900 << "\n";
                }
                if(Borrows.is_open()) Borrows.close();
            }
            catch(exception &err) {
                Error = true;
                system("cls");
                cout << "Wprowadzono bledne ID!";
                Sleep(2000);
                system("cls");
                return;
            }
        }
    }
//----------------------------------------------------------------------------------------------------------------------------------------------List Data
    string ListData(short DATA, string status) {
        string listing = "";
        int i=0;
        int imax = getDataAmount(DATA);
        if(DATA == 0) {
            do {
                RetrieveData(DATA, i);
                if(this->status == status) {
                listing = listing + "ID: " + id + "\nTytul: " + title + "\nAutor: " + author + "\nRok wydania: " + date + "\n\n";
                }
                i++;
            }
            while(i<imax);
        }
        else if(DATA == 1) {
            do {
                RetrieveData(DATA, i);
                if(this->status == status) {
                listing = listing + "ID: " + id + "\nPESEL: " + pesel + "\nImie i nazwisko: " + name + "\nNr telefonu: " + phone + "\nMiejsce zamieszkania: " + por + "\n\n";
                }
                i++;
            }
            while(i<imax);
        }
        else if(DATA == 2) {
            do {
                RetrieveData(DATA, i);
                if(this->status == status) {
                listing = listing + "ID: " + id + "\nID czytelnika wypo¿yczaj¹cego: " + borrow_reader_id + "\nID ksi¹¿ki wypo¿yczonej: " + borrow_book_id + "\nData wypo¿yczenia: " + borrow_date + "\n\n";
                }
                i++;
            }
            while(i<imax);
        }
        if(listing == "") {cout << "Nie znaleziono zadnych danych!\n";}
        return listing;
    }
//----------------------------------------------------------------------------------------------------------------------------------------------Search Data
    string SearchData(short DATA, string filter, string status) {
        string found = "";
        int i=0;
        int imax = getDataAmount(DATA);
        if(DATA == 0) {
            do {
                RetrieveData(DATA, i);
                if(this->status == status) {
                    if(filter == id || filter == title || filter == author || filter == date) {
                        found = found + "ID: " + id + "\nTytul: " + title + "\nAutor: " + author + "\nRok wydania: " + date + "\n\n";
                    }
                }
                i++;
            }
            while(i<imax);
        }
        else if(DATA == 1) {
            do {
                RetrieveData(DATA, i);
                if(this->status == status) {
                    if(filter == id || filter == pesel || filter == name || filter == phone || filter == por) {
                        found = found + "ID: " + id + "\nPESEL: " + pesel + "\nImie i nazwisko: " + name + "\nNr telefonu: " + phone + "\nMiejsce zamieszkania: " + por + "\n\n";
                    }
                }
                i++;
            }
            while(i<imax);
        }
        else if(DATA == 2) {
            do {
                RetrieveData(DATA, i);
                if(this->status == status) {
                    if(filter == id || filter == borrow_reader_id || filter == borrow_book_id || filter == borrow_date) {
                        found = found + "ID: " + id + "\nID czytelnika wypo¿yczaj¹cego: " + borrow_reader_id + "\nID ksi¹¿ki wypo¿yczonej: " + borrow_book_id + "\nData wypo¿yczenia: " + borrow_date + "\n\n";
                    }
                }
                i++;
            }
            while(i<imax);
        }

        if(found == "") return "Nie znaleziono zadnych danych!\n";
        return found;
    }
//----------------------------------------------------------------------------------------------------------------------------------------------Data Status
    void DataStatus(short DATA, string id, string status) {
        Error = false;
        Change = false;
        string newlist = "";
        int i=0;
        try {
            int check = stoi(id);
        }
        catch(exception &err) {
            Error = true;
            system("cls");
            cout << "Wprowadzono bledne ID!";
            cin.clear();
            Sleep(2000);
            system("cls");
            return;
        }
        if(DATA == 0) {
            do{
                RetrieveData(DATA, i);
                if(this->id != id) {
                        newlist = newlist + this->status + "\n" + this->id + "\n" + title + "\n" + author + "\n" + date + "\n";
                }
                else if(this->id == id) {
                        newlist = newlist + status + "\n" + this->id + "\n" + title + "\n" + author + "\n" + date + "\n";
                        Change = true;
                }
                i++;
            }
            while(i<getDataAmount(DATA));
            ofstream Books;
            Books.open("Books.bin", ios::binary);
            if(Books.is_open() && !Books.eof()) {
                Books << newlist;
            }
            if(Books.is_open()) Books.close();
        }
        else if(DATA == 1) {
            do{
                RetrieveData(DATA, i);
                if(this->id != id) {
                        newlist = newlist + this->status + "\n" + this->id + "\n" + pesel + "\n" + name + "\n" + phone + "\n" + por + "\n";
                }
                else if(this->id == id) {
                        newlist = newlist + status + "\n" + this->id + "\n" + pesel + "\n" + name + "\n" + phone + "\n" + por + "\n";
                        Change = true;
                }
                i++;
            }
            while(i<getDataAmount(DATA));
            ofstream Readers;
            Readers.open("Readers.bin", ios::binary);
            if(Readers.is_open() && !Readers.eof()) {
                Readers << newlist;
            }
            if(Readers.is_open()) Readers.close();
        }
        else if(DATA == 2) {
            do{
                RetrieveData(DATA, i);
                if(this->id != id) {
                        newlist = newlist + this->status + "\n" + this->id + "\n" + borrow_reader_id + "\n" + borrow_book_id + "\n" + borrow_date + "\n";
                }
                else if(this->id == id) {
                        newlist = newlist + status + "\n" + this->id + "\n" + borrow_reader_id + "\n" + borrow_book_id + "\n" + borrow_date + "\n";
                        DataStatus(0, borrow_book_id, "ACTIVE");
                        Change = true;
                }
                i++;
            }
            while(i<getDataAmount(DATA));
            ofstream Borrows;
            Borrows.open("Borrows.bin", ios::binary);
            if(Borrows.is_open() && !Borrows.eof()) {
                Borrows << newlist;
            }
            if(Borrows.is_open()) Borrows.close();
        }
    }
};

class Menu : DataMaster {
public:
    void Listing(short DATA, string Status) {
        system("cls");
        cin.ignore();
        cout << getListData(DATA, Status) << endl;
        cout << "Nacisnij ENTER aby wrocic do menu.";
        cin.get();
        system("cls");
    }

    void Searching(short DATA, string status) {
        system("cls");
        cin.ignore();
        if(DATA == 0) cout << "Wprowadz filtr wedlug ktorego szukac ksiazek: ";
        else if(DATA == 1) cout << "Wprowadz filtr wedlug ktorego szukac czytelnikow: ";
        else if(DATA == 2) cout << "Wprowadz filtr wedlug ktorego szukac wypozyczen: ";
        getline(cin, filter);
        system("cls");
        cout << "Wyniki wyszukiwania za pomoca filtru '" << filter << "':\n\n";
        cout << getSearchData(DATA, filter, status) << endl;
        cout << "Nacisnij ENTER aby wrocic do menu.";
        cin.clear();
        cin.ignore(1000, '\n');
        system("cls");
    }

    void StatusChanging(short DATA, string status) {
        system("cls");
        cin.ignore();
        if(DATA == 0) cout << "Wprowadz ID ksiazki do usuniecia: ";
        else if(DATA == 1) cout << "Wprowadz ID czytelnika do usuniecia: ";
        else if(DATA == 2) cout << "Wprowadz ID wypozyczenia do usuniecia: ";
        getline(cin, id);
        setDataStatus(DATA, id, "DELETED");
        if(Error) {return;}
        if(!Change) {
            system("cls");
            cout << "Nie znaleziono obiektu o podanym ID.";
            cin.clear();
            Sleep(2000);
            system("cls");
            return;
        }
        system("cls");
        cout << "Usunieto pomyslnie.\n\nNacisnij ENTER aby wrocic do menu.";
        cin.clear();
        cin.ignore(1000, '\n');
        system("cls");
    }

private:
    string filter;
    int newstatus;
    string typed;
};

int main(int argc, char** argv) {

    SetConsoleCP(852);
    setlocale (LC_ALL, "");

    DataMaster DM;
    Menu menu;

    int Option;

    string ID = "";
    string Title = "";
    string Author = "";
    short Date = 0;

    long long PESEL = 0;
    string Name = "";
    long long Phone = 0;
    string POR = "";

    string Borrow_Reader_ID = "";
    string Borrow_Book_ID = "";

    string Filter = "";

    do {
        cout << "Wybierz opcje z listy\n\n1. Dodaj ksiazke\n2. Listuj ksiazki\n3. Wyszukaj ksiazki\n4. Usun ksiazke\n\n5. Dodaj czytelnika\n6. Listuj czytelnikow\n7. Wyszukaj czytelnikow\n8. Usun czytelnika\n\n9. Zanotuj wypozyczenie\n10. Listuj wypozyczenia\n11. Wyszukaj wypozyczenia\n12. Zwroc ksiazke\n\n13. Wyjdz z programu\n\nTwoj wybor: ";
        cin >> Option;
        switch(Option) {
            case 1:
                cin.clear();
                cin.ignore(1000, '\n');
                system("cls");
                cout << "Wprowadz tytul ksiazki: ";
                getline(cin, Title);
                cout << endl;
                cout << "Wprowadz autora ksiazki: ";
                getline(cin, Author);
                cout << endl;
                cout << "Wprowadz date wydania ksiazki: ";
                cin >> Date;
                if(cin.fail() || Date<1 || Date>2021) {
                    system("cls");
                    cout << "Wprowadzono bledny rok wydania!";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    Sleep(2000);
                    system("cls");
                    break;
                }
                DM.setAddData(0, Title, Author, Date, 0, 0);
                system("cls");
                break;

            case 2:
                menu.Listing(0, "ACTIVE");
                break;

            case 3:
                menu.Searching(0, "ACTIVE");
                break;

            case 4:
                menu.StatusChanging(0, "DELETED");
                break;

            case 5:
                cin.clear();
                cin.ignore(1000, '\n');
                system("cls");
                cout << "Wprowadz PESEL czytelnika: ";
                cin >> PESEL;
                if(cin.fail() || PESEL < 10000000000 || PESEL > 99999999999) {
                    system("cls");
                    cout << "Wprowadzono bledny numer PESEL!";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    Sleep(2000);
                    system("cls");
                    break;
                }
                cin.clear();
                cin.ignore(1000, '\n');
                cout << endl;
                cout << "Wprowadz imie i nazwisko czytelnika: ";
                getline(cin, Name);
                cout << endl;
                cout << "Wprowadz numer telefonu czytelnika: ";
                cin >> Phone;
                if(cin.fail() || Phone < 100000000 || Phone > 999999999) {
                    system("cls");
                    cout << "Wprowadzono bledny numer telefonu!";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    Sleep(2000);
                    system("cls");
                    break;
                }
                cin.clear();
                cin.ignore(1000, '\n');
                cout << endl;
                cout << "Wprowadz miejsce zamieszkania czytelnika: ";
                getline(cin, POR);
                DM.setAddData(1, Name, POR, 0, PESEL, PESEL);
                system("cls");
                break;

            case 6:
                menu.Listing(1, "ACTIVE");
                break;

            case 7:
                menu.Searching(1, "ACTIVE");
                break;

            case 8:
                menu.StatusChanging(1, "DELETED");
                break;

            case 9:
                cin.clear();
                cin.ignore(1000, '\n');
                system("cls");
                cout << "Wprowadz ID czytelnika wypozyczajacego: ";
                getline(cin, Borrow_Reader_ID);
                cout << endl;
                cout << "Wprowadz ID ksiazki wypozyczanej: ";
                getline(cin, Borrow_Book_ID);
                DM.setAddData(2, Borrow_Reader_ID, Borrow_Book_ID, 0, 0, 0);
                system("cls");
                break;

            case 10:
                menu.Listing(2, "ACTIVE");
                break;

            case 11:
                menu.Searching(2, "ACTIVE");
                break;

            case 12:
                menu.StatusChanging(2, "RETURNED");
                break;

            case 13:
                system("cls");
                return 0;

            default:
                system("cls");
                cout << "Wybrana opcja nie istnieje!";
                cin.clear();
                cin.ignore(1000, '\n');
                Sleep(1000);
                system("cls");
                break;
        }
    }
    while(true);
}
