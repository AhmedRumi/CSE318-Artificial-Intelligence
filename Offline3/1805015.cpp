#include<bits/stdc++.h>
using namespace std;
typedef long long ll;


class Course
{
public:
    int course_id;
    int timeslot;
    int no_of_students;
    int status;

    vector<int>conflictingCourses;

    Course(int course_id,int no_of_students)
    {
        this->course_id=course_id;
        this->no_of_students=no_of_students;
        this->timeslot=-1;
        status=0;

    }

    void addConflictingCourse(int index)
    {
        if(find(conflictingCourses.begin(),conflictingCourses.end(),index)==conflictingCourses.end())
        {
            conflictingCourses.push_back(index);
        }

    }



};

class Student
{
public:
    int student_id;
    vector<int>EnrolledCourses;

    Student(int id)
    {
        student_id=id;
    }

    void addCourse(int index)
    {
        EnrolledCourses.push_back(index);
    }

};

vector<Course>courses;
vector<Student>students;


bool cmp1(int c1,int c2) {
    return courses[c1].conflictingCourses.size()>courses[c2].conflictingCourses.size();
}
bool cmp2(int c1,int c2) {
    return courses[c1].no_of_students>courses[c2].no_of_students;
}

class ConstructiveHeuristicSolver
{
public:

    int generateTimetable(vector<int>& order)
    {
        int time_slot_count=-1,n=courses.size();

        for(int i=0;i<n;i++)
        {
            int ind=order[i];
            //cout<<courses[0].timeslot<<"  "<<courses[1].timeslot<<"  "<<courses[2].timeslot<<endl;
            vector<bool>col(n,false);
            //cout<<courses[i].course_id<<" "<<courses[i].conflictingCourses.size()<<endl;
            for(int j=0;j<courses[ind].conflictingCourses.size();j++)
            {
                int temp=courses[courses[ind].conflictingCourses[j]].timeslot;
                //cout<<i<<"  "<<courses[i].conflictingCourses[j]->conflictingCourses.size()<<endl;
                //cout<<temp<<endl;
                if(temp!=-1)
                {
                    col[temp]=true;

                }

            }

           for(int j=0;j<n;j++)
           {
               if(col[j]==false)
               {
                   courses[ind].timeslot=j;
                   //cout<<j<<endl;
                   time_slot_count=max(time_slot_count,j);
                   break;
               }
           }
           //cout<<courses[i].timeslot<<endl;
        }

        return time_slot_count+1;
    }

    int TimetablebyLargestDegree()
    {
        vector<int>order;
        for(int i=0;i<courses.size();i++)order.push_back(i);
        sort(order.begin(),order.end(),cmp1);
        return generateTimetable(order);
    }

    int TimetablebyLargestEnrollment()
    {
        vector<int>order;
        for(int i=0;i<courses.size();i++)order.push_back(i);
        sort(order.begin(),order.end(),cmp2);
        return generateTimetable(order);
    }

    int TimetablebyDSatur()
    {
        int n=courses.size();
        int slots=-1;

        vector<int>sat(n),deg(n);

        for(int i=0;i<n;i++)
        {
            set<int>s;int uncol_degree=0;
            for(int j=0;j<courses[i].conflictingCourses.size();j++)
            {
                Course c=courses[courses[i].conflictingCourses[j]];
                if(c.timeslot!=-1)s.insert(c.timeslot);
                else uncol_degree++;
            }
            sat[i]=s.size();
            deg[i]=uncol_degree;
        }

        for(int k=0;k<n;k++)
        {
            //cout<<k<<" "<<n<<endl;
            int id=-1;
            int mx_sat=-1,mx_deg=-1;
            for(int i=0;i<n;i++)
            {
                if(courses[i].timeslot!=-1)continue;
                if(sat[i]>mx_sat || sat[i]==mx_sat && deg[i]>mx_deg)
                {
                    id=i;
                    mx_sat=sat[i];
                    mx_deg=deg[i];
                }
            }
            //cout<<k<<" "<<id<<endl;
            vector<bool>used(n,false);
            for(int i=0;i<courses[id].conflictingCourses.size();i++)
            {
                Course c=courses[courses[id].conflictingCourses[i]];
                if(c.timeslot!=-1)used[c.timeslot]=true;
            }
            //cout<<k<<endl;
            for(int i=0;i<n;i++)
            {
                if(used[i]==false)
                {
                    courses[id].timeslot=i;
                    slots=max(slots,i);
                    break;
                }
            }
            //cout<<k<<endl;
            for(int i=0;i<courses[id].conflictingCourses.size();i++)
            {
                int cid=courses[id].conflictingCourses[i];
                deg[cid]--;

                int flag=0;
                for(int j=0;j<courses[cid].conflictingCourses.size();j++)
                {
                    int x=courses[cid].conflictingCourses[j];
                    if(x!=id && courses[x].timeslot==courses[id].timeslot)
                    {
                        flag=1;break;
                    }
                }

                if(flag==0)sat[cid]++;

            }
        }

        return slots+1;
    }

    int TimetablebyRandom()
    {

        vector<int>order;
        for(int i=0;i<courses.size();i++)
        {
            order.push_back(i);
        }
        random_shuffle(order.begin(),order.end());
        return generateTimetable(order);
    }

};

double CalculatePenalty(string type)
{
    int n=students.size();
    double penalty=0;

    for(int i=0;i<students.size();i++)
    {
        //cout<<"1111"<<endl;
        vector<int>t;
        for(int j=0;j<students[i].EnrolledCourses.size();j++)
        {
            Course c=courses[students[i].EnrolledCourses[j]];
            t.push_back(c.timeslot);
            //cout<<c.timeslot<<endl;
        }
        //cout<<1<<endl;
        sort(t.begin(),t.end());
        for(int j=0;j<t.size()-1;j++)
        {
            if(t[j+1]-t[j]<=5)
            {
                if(type=="expo")
                {
                   penalty+=pow(2,5-t[j+1]+t[j]);
                }
                else
                {
                    penalty+=2*(5-t[j+1]+t[j]);
                }
            }
        }
    }

    penalty=penalty/n;
    return penalty;
}

class PerturburativeHeuristicSolver
{
public:

    void DFS(int id,int neighbour_slot)
    {
        courses[id].status=1;

        for(int i=0;i<courses[id].conflictingCourses.size();i++)
        {
            int z=courses[id].conflictingCourses[i];
            if(courses[z].status==0 && courses[z].timeslot==neighbour_slot)
            {
                DFS(z,courses[id].timeslot);
            }
        }
    }

    void KempeChainInterchange(int courseid,int neighbour_slot,string penalty_type)
    {
        int current=rand()%courses.size();

        DFS(courseid,neighbour_slot);
        int own_slot=courses[courseid].timeslot;
        //cout<<"    "<<courses[courseid].timeslot<<" "<<neighbour_slot<<endl;
        double prev_penalty=CalculatePenalty(penalty_type);

        for(int i=0;i<courses.size();i++)
        {
            if(courses[i].status==1)
            {
                //cout<<i<<" "<<courses[i].timeslot<<endl;
                if(courses[i].timeslot==own_slot)
                {
                    courses[i].timeslot=neighbour_slot;
                }
                else
                {
                    courses[i].timeslot=own_slot;
                    //cout<<i<<" "<<courses[i].timeslot<<endl;
                }
                //cout<<i<<" "<<courses[i].timeslot<<endl;
            }
        }

        double curr_penalty=CalculatePenalty(penalty_type);
        //cout<<prev_penalty<<" "<<curr_penalty<<endl;
        if(prev_penalty<=curr_penalty)
        {
            for(int i=0;i<courses.size();i++)
            {
                if(courses[i].status==1)
                {
                    //cout<<i<<" "<<courses[i].timeslot<<endl;
                    if(courses[i].timeslot==own_slot)
                    {
                        courses[i].timeslot=neighbour_slot;
                    }
                    else
                    {
                        courses[i].timeslot=own_slot;
                    }
                }
            }
        }

        for(int i=0;i<courses.size();i++)
        {
            if(courses[i].status==1)
            {
                courses[i].status=0;
            }
        }
    }

    void PairSwapOperator(int course_id1,int course_id2,string penalty_type)
    {
        int time_slot1=courses[course_id1].timeslot;
        int time_slot2=courses[course_id2].timeslot;

        if(time_slot1==time_slot2)
        {
            return;
        }

        for(int i=0;i<courses[course_id1].conflictingCourses.size();i++)
        {
            Course c=courses[courses[course_id1].conflictingCourses[i]];
            if(c.timeslot==time_slot2)
            {
                return;
            }
        }

        for(int i=0;i<courses[course_id2].conflictingCourses.size();i++)
        {
            Course c=courses[courses[course_id2].conflictingCourses[i]];
            if(c.timeslot==time_slot1)
            {
                return;
            }
        }

        double prev_penalty=CalculatePenalty(penalty_type);

        courses[course_id1].timeslot=time_slot2;
        courses[course_id2].timeslot=time_slot1;

        double curr_penalty=CalculatePenalty(penalty_type);

        if(prev_penalty<=curr_penalty)
        {
            courses[course_id1].timeslot=time_slot1;
            courses[course_id2].timeslot=time_slot2;
        }
    }

    void ReducePenalty(string type,string penalty_type)
    {
        for(int i=1;i<=5000;i++)
        {
            if(type=="kempe")
            {
                if(i%100==0)cout<<i<<endl;
                int course_id=rand()%courses.size();
                if(!courses[course_id].conflictingCourses.empty())
                {
                    int sz=courses[course_id].conflictingCourses.size();
                    int neighbour_id=courses[course_id].conflictingCourses[rand()%sz];
                    KempeChainInterchange(course_id,courses[neighbour_id].timeslot,penalty_type);
                }
            }
            else
            {
                int course_id1=rand()%courses.size();
                int course_id2=rand()%courses.size();
                PairSwapOperator(course_id1,course_id2,penalty_type);
            }
        }
    }
};


class Solver
{
public:

    void SolvebyScheme(int scheme_no)
    {
        if(scheme_no==1) SolvebyScheme1();
        else if(scheme_no==2) SolvebyScheme2();
        else if(scheme_no==3) SolvebyScheme3();
        else if(scheme_no==4) SolvebyScheme4();
        else SolvebyScheme5();

        PerturburativeHeuristicSolver p;
        double penalty;
        if(scheme_no==5)
        {
            p.ReducePenalty("kempe","linear");
            cout<<"After KempeChainInterchange"<<endl;
            penalty=CalculatePenalty("linear");
            cout<<"penalty: "<<penalty<<endl;

            p.ReducePenalty("pairswap","linear");
            cout<<"After PairSwapOperator"<<endl;
            penalty=CalculatePenalty("linear");
            cout<<"penalty: "<<penalty<<endl;
        }
        else
        {
            p.ReducePenalty("kempe","expo");
            cout<<"After KempeChainInterchange"<<endl;
            penalty=CalculatePenalty("expo");
            cout<<"penalty: "<<penalty<<endl;

            p.ReducePenalty("pairswap","expo");
            cout<<"After PairSwapOperator"<<endl;
            penalty=CalculatePenalty("expo");
            cout<<"penalty: "<<penalty<<endl;
        }
    }

    void SolvebyScheme1()
    {
        ConstructiveHeuristicSolver c;
        int slots=c.TimetablebyLargestDegree();
        double penalty=CalculatePenalty("expo");

        cout<<"Largest Degree"<<endl;
        cout<<"time slots: "<<slots<<endl;
        cout<<"penalty: "<<penalty<<endl;
    }

    void SolvebyScheme2()
    {
        ConstructiveHeuristicSolver c;
        int slots=c.TimetablebyDSatur();
        double penalty=CalculatePenalty("expo");

        cout<<"DSatur"<<endl;
        cout<<"time slots: "<<slots<<endl;
        cout<<"penalty: "<<penalty<<endl;
    }

    void SolvebyScheme3()
    {
        ConstructiveHeuristicSolver c;
        int slots=c.TimetablebyLargestEnrollment();
        double penalty=CalculatePenalty("expo");

        cout<<"Largest Enrollment"<<endl;
        cout<<"time slots: "<<slots<<endl;
        cout<<"penalty: "<<penalty<<endl;
    }

    void SolvebyScheme4()
    {
        ConstructiveHeuristicSolver c;
        int slots=c.TimetablebyRandom();
        double penalty=CalculatePenalty("expo");

        cout<<"Random"<<endl;
        cout<<"time slots: "<<slots<<endl;
        cout<<"penalty: "<<penalty<<endl;
    }

    void SolvebyScheme5()
    {
        ConstructiveHeuristicSolver c;
        int slots=c.TimetablebyLargestDegree();
        double penalty=CalculatePenalty("linear");

        cout<<"Not decided"<<endl;
        cout<<"time slots: "<<slots<<endl;
        cout<<"penalty: "<<penalty<<endl;
    }


};

main()
{

    srand(time(0));

    ifstream fin;
    fin.open("dataset/CARF92-crs.txt");

    int course_id,stdno;
    while(!fin.eof())
    {
        fin>>course_id>>stdno;
        Course c(course_id-1,stdno);
        courses.push_back(c);
    }
    fin.close();

    fin.open("dataset/CARF92-stu.txt");

    int i=0;
    string x;
    char *c;

    while(!fin.eof())
    {
        Student s(i);
        students.push_back(s);

        getline(fin,x);
        char* str=const_cast<char*>(x.c_str());
        c=strtok(str," ");

        while(c!=nullptr)
        {
            students[i].addCourse(atoi(c)-1);
            c=strtok(nullptr," ");
        }
        i++;
    }
    cout<<i<<endl;

    for(int i=0;i<students.size();i++)
    {
        for(int j=0;j<students[i].EnrolledCourses.size();j++)
        {
            for(int k=j+1;k<students[i].EnrolledCourses.size();k++)
            {
                courses[students[i].EnrolledCourses[j]].addConflictingCourse(students[i].EnrolledCourses[k]);
                courses[students[i].EnrolledCourses[k]].addConflictingCourse(students[i].EnrolledCourses[j]);
            }
        }
    }

    Solver solver;
    solver.SolvebyScheme(1);
}
