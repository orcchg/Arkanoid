package com.orcchg.arkanoid.surface;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;

import timber.log.Timber;

class Database {
  private static final String TAG = "Arkanoid_Database";
  
  private static final String databaseName = "ArkanoidDatabase.db";
  private SQLiteDatabase mDbHandler;
  
  private static final String PlayersTable = "PlayersTable";
  private static final String CREATE_PLAYERS_TABLE_STATEMENT =
      "CREATE TABLE IF NOT EXISTS " + PlayersTable + "(" +
      "'ID' INTEGER PRIMARY KEY AUTOINCREMENT DEFAULT 0, " +
      "'Name' TEXT DEFAULT \"\");";
  
  private static final int playerID_columnIndex = 0;
  private static final int playerName_columnIndex = 1;
  private long lastStoredPlayerID = 0;
  
  private static final String StatTable = "StatTable";
  private static final String CREATE_STAT_TABLE_STATEMENT =
      "CREATE TABLE IF NOT EXISTS " + StatTable + "(" +
      "'ID' INTEGER PRIMARY KEY AUTOINCREMENT DEFAULT 0, " +
      "'PlayerID' INTEGER DEFAULT 0, " +
      "'Lives' INTEGER DEFAULT 0, " +
      "'Level' INTEGER DEFAULT 0, " +
      "'Score' INTEGER DEFAULT 0, " +
      "'LevelState' TEXT DEFAULT \"\", " +
      "FOREIGN KEY(PlayerID) REFERENCES " + PlayersTable + "(ID));";
  
  private static final int statID_columnIndex = 0;
  private static final int statPlayerID_columnIndex = 1;
  private static final int statLives_columnIndex = 2;
  private static final int statLevel_columnIndex = 3;
  private static final int statScore_columnIndex = 4;
  private static final int statLevelState_columnIndex = 5;
  private long lastStoredStatID = 0;
  
  Database(final Context context) {
    mDbHandler = context.openOrCreateDatabase(databaseName, Context.MODE_PRIVATE, null);
    mDbHandler.execSQL(CREATE_PLAYERS_TABLE_STATEMENT);
    mDbHandler.execSQL(CREATE_STAT_TABLE_STATEMENT);
    
    lastStoredPlayerID = getLastID(PlayersTable);
    ++lastStoredPlayerID;
    
    lastStoredStatID = getLastID(StatTable);
    ++lastStoredStatID;
  }
  
  /* Players table */
  // --------------------------------------------
  long insertPlayer(String name) throws DatabaseException {
    ContentValues values = new ContentValues();
    values.put("ID", lastStoredPlayerID++);
    values.put("Name", name);
    return insert(PlayersTable, values);
  }
  
  boolean updatePlayer(long id, String name) {
    ContentValues values = new ContentValues();
    values.put("Name", name);
    return update(PlayersTable, id, values);
  }
  
  String getPlayer(long id) {
    String result = "";
    String statement = "SELECT * FROM '" + PlayersTable + "' WHERE ID = '" + id + "';";
    Cursor cursor = mDbHandler.rawQuery(statement, null);
    if (cursor.moveToFirst()) {
      result = cursor.getString(playerName_columnIndex);
    } else {
      Timber.w("Table %s has no Player with requested ID: %s", PlayersTable, id);
    }
    cursor.close();
    return result;
  }
  
  long totalPlayers() {
    return totalRows(PlayersTable);
  }
  
  /* Stat table */
  // --------------------------------------------
  static class GameStat {
    final long player_id;
    final int lives;
    final int level;
    final int score;
    final String state;

    GameStat() {
      this(0, 0, 0, 0, "");
    }
    
    GameStat(long player_id, int lives, int level, int score, String state) {
      this.player_id = player_id;
      this.lives = lives;
      this.level = level;
      this.score = score;
      this.state = state;
    }
  }
  
  long insertStat(long player_id, int lives, int level, int score, String state) throws DatabaseException {
    ContentValues values = new ContentValues();
    values.put("ID", lastStoredStatID++);
    values.put("PlayerID", player_id);
    values.put("Lives", lives);
    values.put("Level", level);
    values.put("Score", score);
    values.put("LevelState", state);
    return insert(StatTable, values);
  }
  
  boolean updateStat(long player_id, int lives, int level, int score, String state) {
    String statement = "SELECT * FROM '" + StatTable + "' WHERE PlayerID = '" + player_id + "';";
    Cursor cursor = mDbHandler.rawQuery(statement, null);
    long id = -1;
    if (cursor.moveToFirst()) {
      id = cursor.getLong(statID_columnIndex);
    } else {
      Timber.w("Table %s has no GameStat with requested PlayerID: %s", StatTable, player_id);
      cursor.close();
      return false;
    }
    cursor.close();
    
    ContentValues values = new ContentValues();
    values.put("PlayerID", player_id);
    values.put("Lives", lives);
    values.put("Level", level);
    values.put("Score", score);
    values.put("LevelState", state);
    return update(StatTable, id, values);
  }
  
  GameStat getStat(long player_id) {
    GameStat result = null;
    String statement = "SELECT * FROM '" + StatTable + "' WHERE PlayerID = '" + player_id + "';";
    Cursor cursor = mDbHandler.rawQuery(statement, null);
    if (cursor.moveToFirst()) {
      int lives = cursor.getInt(statLives_columnIndex);
      int level = cursor.getInt(statLevel_columnIndex);
      int score = cursor.getInt(statScore_columnIndex);
      String state = cursor.getString(statLevelState_columnIndex);
      result = new GameStat(player_id, lives, level, score, state);
    } else {
      Timber.w("Table %s has no GameStat with requested PlayerID: %s", StatTable, player_id);
    }
    cursor.close();
    return result;
  }
  
  long totalStatRecords() {
    return totalRows(StatTable);
  }
  
  /* Clean up database */
  // --------------------------------------------
  void deletePlayer(long id) {
    int affected_number = mDbHandler.delete(StatTable, "PlayerID = '" + id + "'", null);
    if (affected_number == 0) {
      Timber.d("No rows were deleted.");
    }
    delete(PlayersTable, id);
  }
  
  void clearStat(long player_id) {
    int affected_number = mDbHandler.delete(StatTable, "PlayerID = '" + player_id + "'", null);
    if (affected_number == 0) {
      Timber.d("No rows were deleted.");
    }
  }
  
  void clearTables() {
    lastStoredPlayerID = 0;
    lastStoredStatID = 0;
    clearTable(StatTable);
    clearTable(PlayersTable);
  }
  
  /* Exceptions */
  // --------------------------------------------------------------------------
  @SuppressWarnings("serial")
  private static class NoPlaceholdersException extends Exception {
    NoPlaceholdersException(String message) {
      super(message);
    }
  }
  
  @SuppressWarnings("serial")
  static class DatabaseException extends Exception {
    DatabaseException(String message) {
      super(message);
    }
  }
  
  /* Private methods */
  // --------------------------------------------------------------------------
  private long getLastID(final String table_name) {
    long last_id = 0L;
    String statement = "SELECT * FROM '" + table_name + "'";
    Cursor cursor = mDbHandler.rawQuery(statement, null);
    if (cursor.moveToLast()) {
      int id_column_index = cursor.getColumnIndexOrThrow("ID");
      last_id = cursor.getInt(id_column_index);
      Timber.d("Read last ID: %s from Table: %s", last_id, table_name);
    } else {
      Timber.d("Table %s in database %s is empty! Assigning 0 to last ID", table_name, databaseName);
    }
    cursor.close();
    return last_id;
  }
  
  private long insert(final String table_name, ContentValues values) throws DatabaseException {
    Timber.d("Insert: table name[%s], values[%s]", table_name, values);
    long rowid = -1L;
    try {
      rowid = mDbHandler.insertOrThrow(table_name, null, values);
    } catch (SQLException e) {
      String message = "Failed to insert into table " + table_name + ", error: " + e.getMessage();
      Timber.e(message);
      throw new DatabaseException(message);
    }
    return rowid;
  }
  
  private boolean update(final String table_name, final long id, ContentValues values) {
    Timber.d("Update: table name[%s], row id[%s], values[%s]", table_name, id, values);
    int affected_number = mDbHandler.update(table_name, values, "ID = '" + id + "'", null);
    if (affected_number == 0) {
      Timber.d("Nothing to be updated.");
      return false;
    } else if (affected_number > 1) {
      String message = "More than one rows have been affected with update. This is invalid behavior.";
      Timber.e(message);
      throw new RuntimeException(message);
    }
    return true;
  }
  
  private boolean delete(final String table_name, final long id) {
    Timber.d("Delete: table name[%s], row id[%s]", table_name, id);
    int affected_number = mDbHandler.delete(table_name, "ID = '" + id + "'", null);
    if (affected_number == 0) {
      Timber.d("No rows were deleted.");
      return false;
    }
    return true;
  }
  
  private boolean clearTable(final String table_name) {
    Timber.d("Clear: table name[%s]", table_name);
    int affected_number = mDbHandler.delete(table_name, "1", null);
    if (affected_number == 0) {
      Timber.d("No rows were deleted.");
      return false;
    }
    return true;
  }
  
  private long totalRows(final String table_name) {
    long rows_total = 0L;
    Timber.d("Total rows: table name[%s]", table_name);
    String statement = "SELECT COUNT(*) FROM '" + table_name + "'";
    Cursor cursor = mDbHandler.rawQuery(statement, null);
    if (cursor.moveToFirst()) {
      rows_total = cursor.getLong(0);
      Timber.d("Number of rows in table %s is %s", table_name, rows_total);
    } else {
      String message = "Table " + table_name + " in database " + databaseName + " is empty!";
      Timber.e(message);
    }
    cursor.close();
    return rows_total;
  }
  
  private String makePlaceholders(int len) throws NoPlaceholdersException {
    if (len < 1) {
      throw new NoPlaceholdersException("No placeholders");
    } else {
      StringBuilder sb = new StringBuilder(len * 2 - 1);
      sb.append("?");
      for (int i = 1; i < len; i++) {
        sb.append(",?");
      }
      return sb.toString();
    }
  }
}
