#include "dbstoragehandler.h"

DBStorageHandler::DBStorageHandler() {} //DBStorageHandler

void DBStorageHandler::connectToDatabase() {
    const QString connectionName = "db";
    QSqlDatabase database;
    if (QSqlDatabase::contains(connectionName)) {
        // Reuse the existing connection
         database = QSqlDatabase::database(connectionName);

        // Open the database connection if it's not already open
        if (!database.isOpen()) {
            if (!database.open()) {
                qDebug() << "Error: Failed to connect to database:" << database.lastError().text();
            } else {
                qDebug() << "Database connection successful!";
            }
        }
    } else {
        // Add a new connection
         database = QSqlDatabase::addDatabase("QPSQL", connectionName);

        // Set connection parameters
          // NOTE: These are custom connection parameters, please, change in order to work for you:
        database.setHostName("localhost");
        database.setPort(5432);
        database.setDatabaseName("postgres");
        database.setUserName("ivon");

        // Open the database connection
        if (!database.open()) {
            qDebug() << "Error: Failed to connect to database:" << database.lastError().text();
        } else {
            qDebug() << "Database connection successful!";
        }
    }
    // Insert new data into the database
    QSqlQuery query(database);

        // Create table if it doesn't exist
            QString createTableQuery = R"(
            CREATE TABLE IF NOT EXISTS asterisms (

                plate TEXT,
                subsquare_top_left_x1 INT,
                subsquare_top_left_y1 INT,
                subsquare_bottom_right_x2 INT,
                subsquare_bottom_right_y2 INT,
                msw_top_left_x1 INT,
                msw_top_left_y1 INT,
                msw_bottom_right_x2 INT,
                msw_bottom_right_y2 INT
            )
        )";

            if (!query.exec(createTableQuery)) {
                qDebug() << "Error: Failed to create table:" << query.lastError().text();
                return;
            } else {
                qDebug() << "Table 'asterisms' is ready.";
            }
}//connectToDatabase


// Function to delete records with a specific plate from the database
void DBStorageHandler::deleteRecordsByPlate(const QString& sourcePlate,  QSqlQuery& query) {

       query.prepare("DELETE FROM asterisms WHERE plate = :plate");
       query.bindValue(":plate", sourcePlate);

       if (!query.exec()) {
           qDebug() << "Error: Failed to delete records by plate:" << query.lastError().text();
       } else {
           qDebug() << "Deleted records with plate:" << sourcePlate;
       }
}//deleteRecordsByPlate


// Save records of each found asterism within the database
void DBStorageHandler::saveRectanglesToPostgres(const std::vector<RectangleGlobalCoordinates>& rectangles, std::string plate) {
    // Check if records with a given plate exist
    QSqlDatabase database = QSqlDatabase::database("db");

    if (!database.isOpen()) {
        qDebug() << "Error: Database is not open!";
        return;
    }

    QSqlQuery query(database);
    QString sourcePlate = QString::fromStdString(plate);  // Convert std::string to QString

    deleteRecordsByPlate(sourcePlate, query);

   /*
    Assume we have 'asterisms' db, in which we have 1000 records.
        Check if in the asterisms db,
     there is a plate with the name ROB033_000008 already.
    a) and if we have in the db for any of the records, plate: ROB033_000008,
    then we want to delete those records related with this plate in order to renew them.
    And to insert the new data related to ROB033_000008.
    b) Else if there is no records data related to this plate, ROB033_000008,
    to directly insert them in the db.

    Note that in the db we could have records with plates ROB033_000008, ROB033_000011, ROB033_000012, ROB033_000014 and etc.
    So, we want to target the specific plate!
   */

            for (const auto& rect : rectangles) {
                query.prepare(R"(
                    INSERT INTO asterisms ( plate, subsquare_top_left_x1, subsquare_top_left_y1, subsquare_bottom_right_x2, subsquare_bottom_right_y2, msw_top_left_x1, msw_top_left_y1, msw_bottom_right_x2, msw_bottom_right_y2)
                    VALUES ( :plate, :subsquare_top_left_x1, :subsquare_top_left_y1, :subsquare_bottom_right_x2, :subsquare_bottom_right_y2, :msw_top_left_x1, :msw_top_left_y1, :msw_bottom_right_x2, :msw_bottom_right_y2)
                )");

                 // Bind values
                 query.bindValue(":plate", sourcePlate);
                 query.bindValue(":subsquare_top_left_x1", rect.subSquare.startCol);
                 query.bindValue(":subsquare_top_left_y1", rect.subSquare.startRow);
                 query.bindValue(":subsquare_bottom_right_x2", rect.subSquare.endCol);
                 query.bindValue(":subsquare_bottom_right_y2", rect.subSquare.endRow);
                 query.bindValue(":msw_top_left_x1", rect.topLeft.x);
                 query.bindValue(":msw_top_left_y1", rect.topLeft.y);
                 query.bindValue(":msw_bottom_right_x2", rect.bottomRight.x);
                 query.bindValue(":msw_bottom_right_y2", rect.bottomRight.y);

                if (!query.exec()) {
                    qDebug() << "Error: Failed to insert data:" << query.lastError().text();
                }
            }
            qDebug() << "Rectangles saved to PostgreSQL.";
}//saveRectanglesToPostgres
