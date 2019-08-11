 
#include "server.h"
#include <libpq-fe.h>

#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

int main()
{
	// create socket
	using namespace hiraeth::network;

	{
		int         nFields;
		PGconn *conn = PQconnectdb("dbname = test user = postgres password = 12341234");
		if (PQstatus(conn) != CONNECTION_OK)
		{
			fprintf(stderr, "Connection to database failed: %s",
				PQerrorMessage(conn));
		}
		PGresult   *res;
		res = PQexec(conn, "INSERT INTO players (name, level, exp, job) VALUES ('ChrisSinatra', 10, 5990, 1)");
		if (PQresultStatus(res) != PGRES_COMMAND_OK)
		{
			fprintf(stderr, "INSERT command failed: %s", PQerrorMessage(conn));
			PQclear(res);
		}
		PQclear(res);
		//res = PQexec(conn, "BEGIN");
		//if (PQresultStatus(res) != PGRES_COMMAND_OK)
		//{
		//	fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
		//	PQclear(res);
		//}
		//PQclear(res);
		//res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from pg_database");
		//if (PQresultStatus(res) != PGRES_COMMAND_OK)
		//{
		//	fprintf(stderr, "DECLARE CURSOR failed: %s", PQerrorMessage(conn));
		//	PQclear(res);
		//}
		//PQclear(res);

		//res = PQexec(conn, "FETCH ALL in players");
		//if (PQresultStatus(res) != PGRES_TUPLES_OK)
		//{
		//	fprintf(stderr, "FETCH ALL failed: %s", PQerrorMessage(conn));
		//	PQclear(res);
		//}

		///* first, print out the attribute names */
		//nFields = PQnfields(res);
		//for (int i = 0; i < nFields; i++)
		//	printf("%-15s", PQfname(res, i));
		//printf("\n\n");

		///* next, print out the rows */
		//for (int i = 0; i < PQntuples(res); i++)
		//{
		//	for (int j = 0; j < nFields; j++)
		//		printf("%-15s", PQgetvalue(res, i, j));
		//	printf("\n");
		//}

		//PQclear(res);

		/* close the portal ... we don't bother to check for errors ... */
		res = PQexec(conn, "CLOSE players");
		PQclear(res);

		///* end the transaction */
		//res = PQexec(conn, "END");
		//PQclear(res);

		/* close the connection to the database and cleanup */
		PQfinish(conn);
		//otl_connect::otl_initialize(); // initialize ODBC environment
		//try {

		//	db.rlogon("postgres/12341234");

		//	otl_cursor::direct_exec
		//	(
		//		db,
		//		"drop table test_tab",
		//		otl_exception::disabled // disable OTL exceptions
		//	); // drop table

		//	db.commit();

		//	otl_cursor::direct_exec
		//	(
		//		db,
		//		"create table test_tab(f1 int, f2 varchar(30))"
		//	);  // create table

		//	db.commit();

		//	insert(); // insert records into the table
		//	update(10); // update records in the table
		//	select(8); // select records from the table
		//}

		//catch (otl_exception& p) { // intercept OTL exceptions
		//	std::cerr << p.msg << std::endl; // print out error message
		//	std::cerr << p.stm_text << std::endl; // print out SQL that caused the error
		//	std::cerr << p.sqlstate << std::endl; // print out SQLSTATE message
		//	std::cerr << p.var_info << std::endl; // print out the variable that caused the error
		//}

		//db.logoff(); // disconnect from ODBC
	}

	Server server;
	//server.main_60fps_loop();
	//server.main_block_receive_and_async_send();
	//server.main_block_threaded_queue();
	server.main_60fps_threaded_queue();


	return 0;
}
