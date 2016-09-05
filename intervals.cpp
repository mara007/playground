#include <set>
#include <list>
#include <iostream>
#include <algorithm>

struct Interval
{
    size_t low, hi;

    Interval (size_t x)
    : low (x), hi (x)
    {}

    Interval (size_t l, size_t h)
    : low (l), hi (h)
    {}

    Interval ()
    : low (0), hi (0)
    {}

    Interval (const Interval & rhs)
    : low (rhs.low), hi (rhs.hi)
    {}

    enum CmpState
    {
        CS_LEFT,
        CS_JOIN_LEFT,
        CS_JOIN_RIGHT,
        CS_RIGHT
    };

    static is_join (CmpState c) {return CS_JOIN_RIGHT == c || CS_JOIN_LEFT == c; }

    static CmpState compare (const Interval & orig, const Interval & fresh)
    {
        /*
        //LEFT
        if (orig.low > fresh.hi)
        {
            if (
            return CS_LEFT;
        }

        //JOIN_LEFT
        if (orig.low <= fresh.hi)
            return CS_JOIN_LEFT;

         //RIGHT
        if (orig.hi < fresh.low)
            return CS_RIGHT;

        //JOIN_RIGHT
        if (orig.hi >= fresh.low)
            return CS_JOIN_RIGHT;
        */

        if (orig.low > fresh.low)
        {
            if (orig.low <= fresh.hi + 1)
                return CS_JOIN_LEFT;
            else
                return CS_LEFT;
        } else
        if (orig.low < fresh.low)
        {
            if (orig.hi + 1 >= fresh.low)
                return CS_JOIN_RIGHT;
            else
                return CS_RIGHT;
        } else
            return CS_JOIN_LEFT;
    }

    struct Compare
    {
        operator() (const Interval & lhs, const Interval & rhs)
        {
            return lhs.low < rhs.low;
        }
    };

/*    Interval  operator+ (const Interval & lhs, const Interval &rhs)
    {
        return Interval (std::min (rhs.low, lhs.low), std::max (rhs.hi, lhs.hi));
    }
*/
    Interval  operator+ (const Interval &rhs)
    {
        return Interval (std::min (rhs.low, low), std::max (rhs.hi, hi));
    }
   
};

std::ostream & operator<< (std::ostream & ostr, const Interval & rhs)
{
    ostr << "[" << rhs.low << "," << rhs.hi << "]";
    return ostr;
}
        

/******************* Node ****************************/
class Assembler;
struct Node
{
    Assembler * assembler;

    Interval range;

    Node * left, * right;

    Node (size_t l, size_t h)
    : range (l, h), left (nullptr), right (nullptr)
    {}

    Node (const Interval & i, Assembler * a)
    : assembler (a), range (i), left (nullptr), right (nullptr)
    {}

    Node ()
    : range (), left (nullptr), right (nullptr)
    {}

    //! *** ADD **
    void add (const Interval & i)
    {
        Interval::CmpState s = Interval::compare (range, i);
        
        switch (s)
        {
            case Interval::CS_LEFT :
            {
                std::cout << "CS_LEFT : old:" << range << ", new:" << i << std::endl;
                if (!left)
                {
                    left = new Node (i, assembler);
                    return;
                }

                left->add (i);
            }
            break;

            case Interval::CS_RIGHT :
            {
                std::cout << "CS_RIGHT : old:" << range << ", new:" << i << std::endl;
                if (!right)
                {
                    right = new Node (i, assembler);
                    return;
                }

                right->add (i);
            }
            break;

            case Interval::CS_JOIN_LEFT :
            {
                std::cout << "CS_JOIN_LEFT : old:" << range << ", new:" << i << std::endl;
                range = range + i;
            }
            break;

            case Interval::CS_JOIN_RIGHT :
            {
                std::cout << "CS_JOIN_RIGHT : old:" << range << ", new:" << i << std::endl;
                range = range + i;
            }
            break;


        }
    }

    //! ** CHECK_JOIN **
    /*
    void check_join (Node * n, Interval::CmpState s)
    {
        Node * 
        if (Interval::CS_JOIN_LEFT == s)
        {
        }

    }
    */
};

std::ostream & operator<< (std::ostream & ostr, const Node * rhs)
{
    ostr << rhs->range;

    if (rhs->left)
        ostr << " L:" << rhs->left << ",";
    else
        ostr << " L:<null>";

    if (rhs->right)
        ostr << " R:" << rhs->right << ",";
    else
        ostr << " R:<null>";

    return ostr;
}

/******************* Assembler ****************************/
/*
class Assembler
{
    Node * root;
    std::list <Node *> delete_list;

    public:

    void add_node_to_delete (Node *n)
    {
        delete_list.push_back (n);
    }

    Assembler ()
    : root (nullptr)
    {}

    void add (const Interval & i)
    {
        if (!root)
        {
            root = new Node (i, this);
            return;
        }

        root->add (i);
    }

    void debug_print (std::ostream & ostr)
    {
        ostr << "ROOT:";

        if (!root)
            ostr << "<null>";
        else
            ostr << root;

        ostr << std::endl;
    }
};
*/

class Assembler
{
    typedef std::multiset <Interval, Interval::Compare> IntervalSet;

    IntervalSet m_set;
    
    public:

    void add (const Interval & in)
    {
        std::cout << "\nINSERTING: " << in << std::endl;
        IntervalSet::iterator low, hi, i, tmp;
        i = m_set.insert (in);

        if (m_set.size () <= 2)
            return;

        if (m_set.begin () != i)
            --i;
        std::cout << "11\n";
        hi = m_set.upper_bound (Interval (in.hi + 1));

        std::cout << "22\n";
        /*
        IntervalSet::iterator low = m_set.lower_bound (in),
                              hi  = m_set.upper_bound (Interval (in.hi + 1)),
                              i   = low,
                              tmp;
        */

        int xx = 0;
        std::cout << "==============\n";
        std::cout << "LOW: " << *i;
        if  (m_set.end () != hi)
            std::cout << ", HI: " << *hi;
        else
            std::cout << ", HI: <end>";
        std::cout << std::endl;

        //if (m_set.end () != hi)
        //    ++hi;

        Interval new_range (*i);
        while (++i != hi)
        {
            bool erase {false};

            std::cout << "checking : " << new_range << " and " << *i << std::endl;
            if (Interval::is_join (Interval::compare (new_range, *i)))
            {
                new_range = new_range + *i;

                std::cout << "joining - result = " << new_range << std::endl;

            }

            if (erase)
            {
                tmp = i++;
                m_set.erase (tmp);
            } else
            {
                ++i;
            }

            if (++xx == 10)
                break;
        }

        std::cout << std::endl;

        m_set.erase (low, hi);
        m_set.insert (new_range);

       /* 
        if (m_set.begin () != i)
        {
            IntervalSet::iterator left =  --i,
                                  right = ++i;

            Interval::CmpState c = Interval::compare (left
        }
       */
    }

    void debug_print (std::ostream & ostr)
    {
        for (const Interval & i : m_set)
            ostr << i << ", ";

    }
};

int main (int argc, char ** argv )
{
    std::cout << "******* ASSEMBLER *******\n";

    Interval i;
    Assembler ass;

    i = Interval (71,80);
    ass.add (i);
    std::cout << "ADDED : " << i << std::endl << "ASSEMBLED : ";
    ass.debug_print (std::cout);
    std::cout << std::endl<< std::endl;

    i = Interval (51,60);
    ass.add (i);
    std::cout << "ADDED : " << i << std::endl << "ASSEMBLED : ";
    ass.debug_print (std::cout);
    std::cout << std::endl<< std::endl;


    i = Interval (31,40);
    ass.add (i);
    std::cout << "ADDED : " << i << std::endl << "ASSEMBLED : ";
    ass.debug_print (std::cout);
    std::cout << std::endl<< std::endl;


    i = Interval (1,10);
    ass.add (i);
    std::cout << "ADDED : " << i << std::endl << "ASSEMBLED : ";
    ass.debug_print (std::cout);
    std::cout << std::endl<< std::endl;



    i = Interval (41,50);
    ass.add (i);
    std::cout << "ADDED : " << i << std::endl << "ASSEMBLED : ";
    ass.debug_print (std::cout);
    std::cout << std::endl<< std::endl;

    i = Interval (61,70);
    ass.add (i);
    std::cout << "ADDED : " << i << std::endl << "ASSEMBLED : ";
    ass.debug_print (std::cout);
    std::cout << std::endl<< std::endl;

/*
    i = Interval (51,60);
    ass.add (i);
    std::cout << "ADDED : " << i << std::endl << "ASSEMBLED : ";
    ass.debug_print (std::cout);
    std::cout << std::endl;
*/

    return 0;

}
