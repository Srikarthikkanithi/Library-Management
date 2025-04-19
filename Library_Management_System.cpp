#include<bits/stdc++.h>
using namespace std;
class Member;
class Library;
class Book{
      private:
      string isbn;
      int copiesAvailable;
      int totalCopies;
      public:
      string title;
      string author;
      Book(){
        title="UnknownTitle";
        author="UnknownAuthor";                   //default constructor for Book class
        isbn="ISBN";
        copiesAvailable=0;
        totalCopies=5;
      }
      Book(string title1,string author1,string isbn1,int ca1,int tc1){
        title=title1;
        author=author1;
        isbn=isbn1;                               //parametrised constructor for Book class
        copiesAvailable=ca1;
        totalCopies=tc1;
      }
      Book(Book& b,string isbn1){
          title=b.title;
          author=b.author;
          isbn=isbn1;                              //copy constructor with modified isbn 
          copiesAvailable=b.copiesAvailable;
          totalCopies=b.totalCopies;
      }

      string get_isbn();                            //getter for isbn
      int get_copiesAvailable();                    //getter for copiesavailable
      int get_totalCopies();                        //gatter for totalcopies

      void updateCopies(int count);                  //setter for updating copiesavailable and totalcopies

      bool borrowBook();                              //borrowing book
      bool returnBook();                               //returning book
      void printDetails();                             //printing details of the book

      friend class Member;
      friend class Library;
};
string Book::get_isbn(){
    return isbn;
}
int Book::get_copiesAvailable(){
    return copiesAvailable;
}
int Book::get_totalCopies(){
    return totalCopies;
}
void Book::updateCopies(int count){
    if(count<0){
        if(copiesAvailable>=-count&&totalCopies>=-count){
            copiesAvailable+=count;
            totalCopies+=count;
        }else{
            cout<<"Invalid request! Count becomes negative"<<endl;
        }
    }else{
        copiesAvailable+=count;
        totalCopies+=count;
    }
}
bool Book::borrowBook(){
    if(copiesAvailable>0){
        copiesAvailable-=1;
        return true;
    }else{
        cout<<"Invalid request! Copy of book not available"<<endl;
        return false;
    }
}
bool Book::returnBook(){
    if(copiesAvailable<totalCopies){
        copiesAvailable+=1;
        return true;
    }else{
        cout<<"Invalid request! Copy of book exceeds total copies"<<endl;
        return false;
    }
}
void Book::printDetails(){
     cout<<title<<" "<<author<<endl;
}
class Member{
    private:
    string memberID;
    map<string,int> borrowedBooks;
    int borrowLimit;
    public:
    string name;
    Member()=default;
    Member(string mid1,int bl1,string name1);           //parametrised constructor for Member class
    Member(string mid1,string name1);                    //constructor with default borrowlimit as 3

    bool borrowBook(string isbn);
    bool returnBook(string isbn);
    void printDetails();

    friend class Library;
    
};
Member::Member(string mid1,int bl1,string name1){
    memberID=mid1;
    borrowLimit=bl1;
    name=name1;
}
Member::Member(string mid1,string name1){
    memberID=mid1;
    borrowLimit=3;
    name=name1;
}
bool Member::borrowBook(string isbn){
    if(borrowLimit>0){
        borrowLimit--;
        borrowedBooks[isbn]++;
        return true;
    }else{
        cout<<"Invalid request! Borrow limit exceeded"<<endl;
        return false;
    }
}
bool Member::returnBook(string isbn){
    auto itr=borrowedBooks.find(isbn);
    if(itr!=borrowedBooks.end()){
        if(itr->second>0){
        itr->second--;
        return true;
        }else{
        cout<<"Invalid request! Book not borrowed"<<endl;
        return false;
        }
    }else{
        cout<<"Invalid request! Book not borrowed"<<endl;
        return false;
    }
}
void Member::printDetails(){
    map<string,int> m1=borrowedBooks;
    for(auto& i:m1){
        if(i.second!=0){
            cout<<memberID<<" "<<name<<" "<<i.first<<" "<<i.second<<endl;
        }
    }
}
class  Library{
    private:
    vector<string> book_isbs;
    vector<string> member_ids;
    unordered_map<string,Book> books;
    unordered_map<string,Member> members;
    public:
    bool addBook(Book& b);
    void add_Book_isbn(string isbn1,string isbn2);
    void updatecopies_count(string isbn1,int count1);
    void print_book(string isbn1);
    bool registerMember(Member& m);
    void print_member(string mid1);
    bool borrowBook(string memberID1, string isbn1);
    bool returnBook(string memberID1, string isbn1);
    void printLibraryDetails();
};
bool Library::addBook(Book& b){
     string isbn1=b.isbn;
     auto itr1=books.find(isbn1);
     if(itr1==books.end()||(itr1!=books.end()&&itr1->second.copiesAvailable==0)){
        books[isbn1]=b;
        book_isbs.push_back(isbn1);
        return true;
     }else{
        cout<<"Invalid request! Book with same isbn already exists"<<endl;
        return false;
     }
}
bool Library::registerMember(Member& m){
     string mid1=m.memberID;
     auto itr=members.find(mid1);
     if(itr==members.end()){
        members[mid1]=m;
        member_ids.push_back(mid1);
        return true;
     }else{
        cout<<"Invalid request! Member with same id already exists"<<endl;
        return false;
     }
}
bool Library::borrowBook(string memberID1, string isbn1){
    auto itr1=books.find(isbn1);
    if(itr1!=books.end()&&itr1->second.copiesAvailable>0){
        auto itr2=members.find(memberID1);
        if(itr2->second.borrowLimit>0){
            itr1->second.copiesAvailable--;
            itr2->second.borrowLimit--;
            itr2->second.borrowedBooks[isbn1]++;
            return true;
        }else{
        cout<<"Invalid request! Borrow limit exceeded"<<endl; 
        return false;
        }
    }else{
        cout<<"Invalid request! Copy of book not available"<<endl;
        return false;
    }
}
bool Library::returnBook(string memberID1, string isbn1){
     auto itr1=books.find(isbn1);
     if(itr1->second.copiesAvailable<itr1->second.totalCopies){
        auto itr2=members.find(memberID1);
        map<string,int> m1=itr2->second.borrowedBooks;
        auto itr3=m1.find(isbn1);
        if(itr3!=m1.end()&&itr3->second>0){
            itr1->second.copiesAvailable++;
            itr2->second.borrowLimit++;
            itr2->second.borrowedBooks[isbn1]--;
            return true;
        }else{
        cout<<"Invalid request! Book not borrowed"<<endl;
        return false;
        }
     }else{
        cout<<"Invalid request! Copy of book exceeds total copies"<<endl;
        return false;
     }
}
void Library::printLibraryDetails(){
     for(auto& i:book_isbs){
        auto itr1=books.find(i);
        cout<<itr1->second.title<<" "<<itr1->second.author<<" "<<itr1->second.copiesAvailable<<endl;
     }
     for(auto& i:member_ids){
        auto itr1=members.find(i);
        cout<<itr1->second.memberID<<" "<<itr1->second.name<<endl;
     }
     
}
void Library::add_Book_isbn(string isbn1,string isbn2){
    auto itr1=books.find(isbn1);
    if(itr1!=books.end()){
    Book b1(itr1->second,isbn2);
    books[isbn2]=b1;
    book_isbs.push_back(isbn2);
    }else{
        cout<<"Invalid request! Copy of book not available"<<endl;
    }
}
void Library::updatecopies_count(string isbn1,int count1){
     auto itr1=books.find(isbn1);
     if(itr1!=books.end()){
     itr1->second.updateCopies(count1);
     }else{
        cout<<"Invalid request! Count becomes negative"<<endl;
     }
}
void Library::print_book(string isbn1){
     auto itr1=books.find(isbn1);
     if(itr1!=books.end()){
     itr1->second.printDetails();
     }else{
      cout<<"Invalid request! Copy of book not available"<<endl;
     }
}
void Library::print_member(string mid1){
    auto itr1=members.find(mid1);
    itr1->second.printDetails();
}
int main(){
    Library l;
    while(true){
        string s;
        cin>>s;
        if(s.compare("Book")==0){
            string s1;
            cin>>s1;
            if(s1.compare("None")==0){
              
              Book b1;
              l.addBook(b1);
            }else if(s1.compare("ExistingBook")==0){
              string oldisbn,newisbn;
              cin>>oldisbn>>newisbn;
              l.add_Book_isbn(oldisbn,newisbn);
            }else{
               string s2,s3;
               int i1,i2;
               cin>>s2>>s3>>i1>>i2;
               Book b1(s1,s2,s3,i1,i2);
               l.addBook(b1);
            }

        }else if(s.compare("UpdateCopiesCount")==0){
        string s1;
        int i1;
        cin>>s1>>i1;
        l.updatecopies_count(s1,i1);
        }else if(s.compare("Member")==0){
          string s1;
          cin>>s1;
          if(s1.compare("NoBorrowLimit")==0){
            string s2,s3;
            cin>>s2>>s3;
            Member m1(s2,s3);
            l.registerMember(m1);
          }else{
            string s2;
            int i1;
            cin>>s2>>i1;
            Member m1(s1,i1,s2);
            l.registerMember(m1);
          }
        }else if(s.compare("Borrow")==0){
             string s1,s2;
             cin>>s1>>s2;
             l.borrowBook(s1,s2);
        }else if(s.compare("Return")==0){
             string s1,s2;
             cin>>s1>>s2;
             l.returnBook(s1,s2);
        }else if(s.compare("PrintBook")==0){
              string s1;
              cin>>s1;
              l.print_book(s1);
        }else if(s.compare("PrintMember")==0){
            string s1;
            cin>>s1;
            l.print_member(s1);
        }else if(s.compare("PrintLibrary")==0){
             l.printLibraryDetails();
        }else{
            break;
        }
    }
}