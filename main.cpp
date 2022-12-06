#include <iostream>
#include <iomanip>
#include <bitset>
#include <thread>

#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>


const int ntrey=3;
const int capacity=8; // > 4 ?

struct TreyVal
{
    int val=0;
    bool flag=false; // true when healthy data, false when garbage
};

struct TreyVal producer_worker(char *c)
{
    try
    {
        int val = std::stoi(c);
        return {val, true};
    }
    catch (const std::invalid_argument &ia)
    {
        //std::cerr << "Invalid argument: " << ia.what() << std::endl;
        return {0, false};
    }
};

void consumer_worker(const TreyVal &a, const TreyVal &b, const TreyVal &c)
{

    std::ostringstream strey;
    strey << "(";
    for (int k=0; k < ntrey; ++k)
    {
       TreyVal t = {0,false};
        switch (k)
        {
        case 0:
            t = a;
            break;
        case 1:
            t = b;
            break;
        case 2:
            t = c;
            break;
        }
        // output "( a b c)" in strey with NaN recognition based on trey_flag
        strey << " " << (t.flag? std::to_string(t.val) : "NaN");
    }
    strey  << ") ";
    std::cout << strey.str() <<  std::endl;
};


int main( int argc, char *argv[] ){

  boost::lockfree::queue<TreyVal> queue(capacity);

   auto producer = std::thread([&]
    {
        TreyVal t;
        for (int i = 1; i < argc; ++i)
        {
            t = producer_worker( argv[i] );
            while (!queue.push(t));
            //std::cout << "push:  " << t.val << ", " << t.flag << std::endl;

            while ( !queue.empty() );
        }
    });


   auto consumer = std::thread([&]
    {
        TreyVal a, b, t;
        for (int i = 1; i < argc; ++i)
        {

            int  share_of_trey = (i%ntrey);
            bool trey_ready = (share_of_trey==0);
            bool trey_last = (i==argc-1);

            while (!queue.pop(t));
            //std::cout << "pop: " << t.val << ", " << t.flag << std::endl;

            if(trey_ready) {
                consumer_worker( a, b, t );
                a={0, false};
                b={0, false};
            } else if (share_of_trey==1) {
                if(trey_last) consumer_worker( t, {0, false}, {0,false} );
                a = t;
            } else  {
                if(trey_last) consumer_worker( a, t, {0,false} );
                b = t;
            };           
            
        }
    });

   producer.join();
   consumer.join();
  
  }
