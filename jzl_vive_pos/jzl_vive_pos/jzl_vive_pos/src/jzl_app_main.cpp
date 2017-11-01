/*
**	JetZux Ltd. 2017
**  https://github.com/JetMeta
*/

#include "jzl_pch.h"
#include "jzl_vive_session.h"

void print_help()
{
    using namespace std;

    cout << "ZVivePos : show position info of vive track object." << endl;
    cout << "\t" << "JetZux Ltd. 2017 " << endl;
    cout << "\t" << "https://github.com/JetMeta" << endl << endl;
}

int main()
{
    print_help();

    JzlViveSession vive_session;

    if (!vive_session.init())
    {
        std::cout << "error : init openvr." << std::endl;
    }

    std::cout << "vive track object info : " << std::endl << std::endl;

    vive_session.get_pos_info();

    vive_session.uninit();

    return 0;
}
