#ifndef _SQL_
#define _SQL_

#include <iostream>
#include <string>
#include <mysql.h>
#include <string.h>
#include <stdlib.h>


class sqlApi{
	public:
		sqlApi(const std::string &_h,\
				const int &_port,\
    			const std::string &_u="root",\
    			const std::string &_p="",\
    			const std::string &_db="Linux1_class");
//		sqlApi(const std::string &_h,\
//				const int &_port,\
//				const std::string &_u,\
//				const std::string &_p,\
//				const std::string &_db);
		int connect();
        int insert(const std::string &_name,\
        		   const std::string &_sex,\
        		   const std::string  &_age,\
        		   const std::string &_hobby,\
        		   const std::string &_school);
		int select();
		~sqlApi();
	private:
		sqlApi();
		MYSQL *conn;
		MYSQL_RES *res;
		std::string host;
		std::string user;
		std::string passwd;
		std::string db;
		int port;
};


#endif
