#include <iostream>
#include <ctime>

using namespace std;
int main(){
    time_t currentTime = time(nullptr);
    cout<<currentTime<<endl;
    char *readableTime = ctime(&currentTime);
    cout<<readableTime<<endl;
    
    struct tm *localTime = localtime(&currentTime); 
    cout<< localTime -> tm_mday<<" "
        <<localTime -> tm_hour<<":"
        <<localTime -> tm_min<<":"
        <<localTime -> tm_sec<<" "<<localTime -> tm_year +1900<<endl;
    

    char formattedTime[100];
    strftime(formattedTime , sizeof(formattedTime) , "%Y-%m-%d %H:%M:%S" , localTime);
    cout<<formattedTime<<endl;
    cout<<formattedTime[74]<<endl;
    cout<<"----------------------------"<<endl;

    double difference = difftime(currentTime , currentTime - 3600);
    cout<<difference<<endl;
    return 0;
}


