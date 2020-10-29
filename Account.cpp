#include "Account.h"

void mysort(Transaction** months, int* frequency){
    int i,j,k;
    Transaction temp;
    for(k=0 ; k<12 ; k++){
        for(i=1 ; i<frequency[k] ; i++){
            for(j=0 ; j<frequency[k]-1 ; j++){
                if(months[k][j] > months[k][j+1]){
                    temp = months[k][j];
                    months[k][j] = months[k][j+1];
                    months[k][j+1] = temp;
                }
            }
        }
    }
}



Account::Account(){
    _id = -1;
    _activity = NULL;
    _monthly_activity_frequency = NULL;
}



Account::Account(int id, Transaction** const activity, int* monthly_activity_frequency){
    _id = id;
    _activity = new Transaction*[12];
    for(int i=0 ; i<12 ; i++){
        if(monthly_activity_frequency[i] != 0){
            _activity[i] = new Transaction[monthly_activity_frequency[i]];
            for(int j=0 ; j<monthly_activity_frequency[i] ; j++){
                _activity[i][j] = activity[i][j];
            }
        }
        else _activity[i] = nullptr;
    }
    _monthly_activity_frequency = new int[12];
    for(int i=0 ; i<12 ; i++){
        _monthly_activity_frequency[i] = monthly_activity_frequency[i];
    }
    mysort(_activity, _monthly_activity_frequency);
}



Account::~Account(){
    if(_activity){
        for(int i=0 ; i<12 ; i++){
            if(_activity[i] != NULL) delete[] _activity[i];
        }
        delete[] _activity;
    }
    if(_monthly_activity_frequency) delete[] _monthly_activity_frequency;
    _activity = NULL;
    _monthly_activity_frequency = NULL;
}



Account::Account(const Account& rhs){
    _id = rhs._id;
    _activity = new Transaction*[12];
    for(int i=0 ; i<12 ; i++){
        if(rhs._monthly_activity_frequency[i]!=0){
            _activity[i] = new Transaction[rhs._monthly_activity_frequency[i]];
            for(int j=0 ; j<rhs._monthly_activity_frequency[i] ; j++){
                _activity[i][j] = rhs._activity[i][j];
            }
        }
        else _activity[i] = nullptr;
    }
    _monthly_activity_frequency = new int[12];
    for(int i=0 ; i<12 ; i++){
        _monthly_activity_frequency[i] = rhs._monthly_activity_frequency[i];
    }
}



Account::Account(const Account& rhs, time_t start_date, time_t end_date){
    _id = rhs._id;
    int* list = new int[12];
    _activity = new Transaction*[12];
    for(int i=0 ; i<12 ; i++){
        int k=0;
        if(rhs._monthly_activity_frequency[i]){
            for(int j=0 ; j<rhs._monthly_activity_frequency[i] ; j++){
                if(rhs._activity[i][j] > start_date && rhs._activity[i][j] < end_date){
                    k = k+1;
                }
            }
        }
        list[i]=k;
    }
    _monthly_activity_frequency = new int[12];
    for(int i=0 ; i<12 ; i++){
        _monthly_activity_frequency[i] = list[i];
        if(list[i]) _activity[i] = new Transaction[list[i]];
        else _activity[i] = nullptr;
    }

    for(int i=0 ; i<12 ; i++){
        int k=0;
        if(rhs._monthly_activity_frequency[i]){
            for(int j=0 ; j<rhs._monthly_activity_frequency[i] ; j++){
                if(rhs._activity[i][j] > start_date && rhs._activity[i][j] < end_date){
                    _activity[i][k] = rhs._activity[i][j];
                    k=k+1;
                }
            }
        }
        else _activity[i] = nullptr;
    }
    delete[] list;
}



Account::Account(Account&& rhs){
    if(this != &rhs){
        _id = rhs._id;
        _activity = rhs._activity;
        _monthly_activity_frequency = rhs._monthly_activity_frequency;
        rhs._activity = nullptr;
        rhs._monthly_activity_frequency = nullptr;
        rhs._id = 0;
    }
}



Account& Account::operator=(Account&& rhs){
    if(this != &rhs){
        this->~Account();
        this->_activity = rhs._activity;
        this->_monthly_activity_frequency = rhs._monthly_activity_frequency;
        this->_id = rhs._id;
        rhs._activity = nullptr;
        rhs._monthly_activity_frequency = nullptr;
        rhs._id = 0;
    }
    return *this;
}



Account& Account::operator=(const Account& rhs){
    if(this != &rhs){
        this->~Account();
        _id = rhs._id;
        _activity = new Transaction*[12];
        for(int i=0 ; i<12 ; i++){
            if(rhs._monthly_activity_frequency[i]){
                _activity[i] = new Transaction[rhs._monthly_activity_frequency[i]];
                for(int j=0 ; j<rhs._monthly_activity_frequency[i] ; j++){
                    _activity[i][j] = rhs._activity[i][j];
                }
            }
            else _activity[i] = nullptr;
        }
        _monthly_activity_frequency = new int[12];
        for(int i=0 ; i<12 ; i++){
            _monthly_activity_frequency[i] = rhs._monthly_activity_frequency[i];
        }
    }
    return *this;
}



bool Account::operator==(const Account& rhs) const{
    if(this->_id == rhs._id) return true;
    else return false;
}



bool Account::operator==(int id) const{
    if(this->_id == id) return true;
    else return false;
}



Account& Account::operator+=(const Account& rhs){
    Account* myacc = new Account;
    myacc->_id = this->_id;
    myacc->_monthly_activity_frequency = new int[12];
    myacc->_activity = new Transaction*[12];

    for(int i=0 ; i<12 ; i++){
        myacc->_monthly_activity_frequency[i] = _monthly_activity_frequency[i] + rhs._monthly_activity_frequency[i];
    }

    for(int i=0 ; i<12 ; i++){
        myacc->_activity[i] = new Transaction[_monthly_activity_frequency[i] + rhs._monthly_activity_frequency[i]];
    }

    for(int i=0 ; i<12 ; i++){
        for(int j=0 ; j<_monthly_activity_frequency[i] ; j++){
            myacc->_activity[i][j] = _activity[i][j];
        }
    }

    for(int i=0 ; i<12 ; i++){
        int k = 0;
        for(int j=_monthly_activity_frequency[i] ; j<myacc->_monthly_activity_frequency[i] ; j++){
            myacc->_activity[i][j] = rhs._activity[i][k];
            k = k+1;
        }
    }
    mysort(myacc->_activity, myacc->_monthly_activity_frequency);
    *this = *myacc;
    delete myacc;
    return *this;
}


double Account::balance(){
    double money = 0;
    for(int i=0 ; i<12 ; i++){
        for(int j=0 ; j<this->_monthly_activity_frequency[i] ; j++){
            money = _activity[i][j] + money;
        }
    }
    return money;
}



double Account::balance(time_t end_date){
    double money = 0;
    for(int i=0 ; i<12 ; i++){
        for(int j=0 ; j<this->_monthly_activity_frequency[i] ; j++){
            if(_activity[i][j] < end_date){
                money = _activity[i][j] + money;
            }
        }
    }
    return money;
}



double Account::balance(time_t start_date, time_t end_date){
    double money = 0;
    for(int i=0 ; i<12 ; i++){
        for(int j=0 ; j<this->_monthly_activity_frequency[i] ; j++){
            if(_activity[i][j] > start_date && _activity[i][j] < end_date){
                money = _activity[i][j] + money;
            }
        }
    }
    return money;
}



std::ostream& operator<<(std::ostream& os, const Account& account){
    if(account._activity != nullptr && account._monthly_activity_frequency != nullptr){
        os<<account._id<<"\n";
        for(int i=0 ; i<12 ; i++){
            for(int j=0 ; j<account._monthly_activity_frequency[i] ; j++){
                os<<account._activity[i][j];
            }
        }
    }
    else os<<-1<<"\n";
    return os;
}