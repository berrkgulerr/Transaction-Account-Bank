#include "Bank.h"



Bank::Bank(){
    _bank_name = "not_defined";
    _users = nullptr;
    _user_count = 0;
}



Bank::Bank(std::string bank_name, Account* const users, int user_count){
    _bank_name = bank_name;
    _user_count = user_count;
    _users = new Account[_user_count];
    for(int i=0 ; i<_user_count ; i++){
        _users[i] = users[i];
    }
}



Bank::~Bank(){
    if(_users){
        delete[] _users;
    }
}



Bank::Bank(const Bank& rhs){
    _bank_name = rhs._bank_name;
    _user_count = rhs._user_count;
    if(_user_count != 0){
        _users = new Account[_user_count];
        for(int i=0 ; i<_user_count ; i++){
            _users[i] = rhs._users[i];
        }
    }
}



Bank& Bank::operator+=(const Bank& rhs){
    Bank* mybank = new Bank;
    mybank->_bank_name = this->_bank_name;
    int lol=0;
    int p=0;
    for(int i=0; i<_user_count ; i++){
        if(_users[i] == rhs._users[i]) lol++;
    }
    p = _user_count + rhs._user_count - lol;
    mybank->_user_count = p;
    if(p != 0){
        mybank->_users = new Account[p];
        int k=0;
        for(int i=0 ; i<_user_count ; i++){
            mybank->_users[i] = _users[i];
            k++;
        }
        for(int j=0 ; j<rhs._user_count ; j++){
            int x=0;
            for(int a=0 ; a<_user_count ; a++){
                if(mybank->_users[a] == rhs._users[j]){
                    mybank->_users[a] += rhs._users[j];
                    x++;
                }
            }
            if(x==0){
                mybank->_users[k] = rhs._users[j];
                k++;
            } 
        }
    }
    if(_users) delete[] _users;
    this->_bank_name = mybank->_bank_name;
    this->_user_count = mybank->_user_count;
    if(_user_count){
        this->_users = new Account[_user_count];
        for(int i=0 ; i<_user_count ; i++){
            _users[i] = mybank->_users[i];
        }
    }
    delete mybank;
    return *this;
}



Bank& Bank::operator+=(const Account& new_acc){
    for(int i=0 ; i<_user_count ; i++){
        if(_users[i] == new_acc){
            _users[i]+=new_acc;
            return *this;
        }
    }
    int k = _user_count + 1;
    Account* myusers = new Account[k];
    for(int i=0 ; i<_user_count ; i++) myusers[i] = _users[i];
    myusers[_user_count] = new_acc;
    delete[] _users;
    _users = new Account[k];
    for(int i=0 ; i<k ; i++) _users[i] = myusers[i];
    _user_count = k;
    delete[] myusers;
    return *this;
}



Account& Bank::operator[](int account_id){
    int k=0;
    for(int i=0 ; i<_user_count ; i++){
        if(_users[i] == account_id) return _users[i];
    }
    return _users[0];
}



std::ostream& operator<<(std::ostream& os, const Bank& bank){
    int k = bank._user_count;
    int balance = 0;
    for(int i=0 ; i<bank._user_count ; i++){
        int a=0;
        for(int month=0 ; month<12 ; month++){
            struct tm some_date1 = {0};
            some_date1.tm_hour = 0;   some_date1.tm_min = 0; some_date1.tm_sec = 0;
            some_date1.tm_year = 119; some_date1.tm_mon = month; some_date1.tm_mday = 1;

            struct tm some_date2 = {0};
            some_date2.tm_hour = 0;   some_date2.tm_min = 0; some_date2.tm_sec = 0;
            some_date2.tm_year = 119; some_date2.tm_mon = month + 1; some_date2.tm_mday = 1;
    
            time_t mytime1 = mktime(&some_date1);
            time_t mytime2 = mktime(&some_date2);
            if(bank._users[i].balance(mytime1, mytime2) < 0) a++;
            else a = 0;
            if(a==2){
                k--;
                break;
            }
        }
        balance = balance + bank._users[i].balance();
    }
    os<<bank._bank_name<<'\t'<<k<<'\t'<<balance<<"\n";
    return os;
}