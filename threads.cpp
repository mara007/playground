#include <iostream>
#include <thread>
#include <mutex>


class MyCout
{
    std::mutex m_lock;

    MyCout (MyCout & rhs) = delete;
    MyCout () = default;


public:
    template <class T>
    MyCout & operator<< (const T & x)
    {
    //    if (m_lock.try_lock ())
        {
            std::cout << x;
    //        m_lock.unlock ();
        }

        return *this;
    }

    static MyCout & instance ()
    {
        static MyCout m_instance;
        return m_instance;
    }

    void lock ()
    {
        m_lock.lock();
    }

    void unlock ()
    {
        m_lock.unlock ();
    }

    void flush ()
    {
        std::cout << std::flush;
    }

};


void thread_func(const std::string & what, int sleep)
{
    for (int i =0 ; i < 100; ++i)
    {
        MyCout::instance ().lock ();
        MyCout::instance () << what << i << " ";

        MyCout::instance ().flush ();
        MyCout::instance ().unlock ();

        std::this_thread::sleep_for (std::chrono::milliseconds (sleep));
    }

    MyCout::instance ().lock ();
    MyCout::instance () << what << "-done! ";

    MyCout::instance ().flush ();
    MyCout::instance ().unlock ();

}

int main (int argc, char * argv [])
{
    std::cout << "Hello world! of git" << std::endl;


    std::thread t1(thread_func, (std::string("a")), 10);
    std::thread t2(thread_func, (std::string("b")), 30);

    t1.join ();
    t2.join ();
    return 0;
}

