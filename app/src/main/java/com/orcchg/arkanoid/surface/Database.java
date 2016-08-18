package com.orcchg.arkanoid.surface;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;

class Database {
  private static final String TAG = "Arkanoid_Database";
  
  static final String databaseName = "ArkanoidDatabase.db";
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
    String statement = "SELECT * FROM '" + PlayersTable + "' WHERE ID = '" + id + "';";
    Cursor cursor = mDbHandler.rawQuery(statement, null);
    if (cursor.moveToFirst()) {
      return cursor.getString(playerName_columnIndex);
    } else {
      Log.w(TAG, "Table " + PlayersTable + " has no Player with requested ID: " + id);
      return "";
    }
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
      Log.w(TAG, "Table " + StatTable + " has no GameStat with requested PlayerID: " + player_id);
      return false;
    }
    
    ContentValues values = new ContentValues();
    values.put("PlayerID", player_id);
    values.put("Lives", lives);
    values.put("Level", level);
    values.put("Score", score);
    values.put("LevelState", state);
    return update(StatTable, id, values);
  }
  
  GameStat getStat(long player_id) {
    String statement = "SELECT * FROM '" + StatTable + "' WHERE PlayerID = '" + player_id + "';";
    Cursor cursor = mDbHandler.rawQuery(statement, null);
    if (cursor.moveToFirst()) {
      int lives = cursor.getInt(statLives_columnIndex);
      int level = cursor.getInt(statLevel_columnIndex);
      int score = cursor.getInt(statScore_columnIndex);
      String state = cursor.getString(statLevelState_columnIndex);
      return new GameStat(player_id, lives, level, score, state);
    } else {
      Log.w(TAG, "Table " + StatTable + " has no GameStat with requested PlayerID: " + player_id);
      return null;
    }
  }
  
  long totalStatRecords() {
    return totalRows(StatTable);
  }
  
  /* Clean up database */
  // --------------------------------------------
  void deletePlayer(long id) {
    int affected_number = mDbHandler.delete(StatTable, "PlayerID = '" + id + "'", null);
    if (affected_number == 0) {
      Log.d(TAG, "No rows were deleted.");
    }
    delete(PlayersTable, id);
  }
  
  void clearStat(long player_id) {
    int affected_number = mDbHandler.delete(StatTable, "PlayerID = '" + player_id + "'", null);
    if (affected_number == 0) {
      Log.d(TAG, "No rows were deleted.");
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
  public static class NoPlaceholdersException extends Exception {
    public NoPlaceholdersException(String message) {
      super(message);
    }
  }
  
  @SuppressWarnings("serial")
  public static class DatabaseException extends Exception {
    public DatabaseException(String message) {
      super(message);
    }
  }
  
  /* Private methods */
  // --------------------------------------------------------------------------
  private long getLastID(final String table_name) {
    String statement = "SELECT * FROM '" + table_name + "'";
    Cursor cursor = mDbHandler.rawQuery(statement, null);
    if (cursor.moveToLast()) {
      int id_column_index = cursor.getColumnIndexOrThrow("ID");
      int last_id = cursor.getInt(id_column_index);
      Log.d(TAG, "Read last ID: " + last_id + ", from Table: " + table_name);
      return last_id;
    } else {
      Log.d(TAG, "Table " + table_name + " in database " + databaseName + " is empty! Assigning 0 to last ID");
      return 0;
    }
  }
  
  private long insert(final String table_name, ContentValues values) throws DatabaseException {
    Log.d(TAG, "Insert: table name[" + table_name + "], values[" + values + "]");
    long rowid = -1;
    try {
      rowid = mDbHandler.insertOrThrow(table_name, null, values);
    } catch (SQLException e) {
      String message = "Failed to insert into table " + table_name + ", error: " + e.getMessage();
      Log.e(TAG, message);
      throw new DatabaseException(message);
    }
    return rowid;
  }
  
  private boolean update(final String table_name, final long id, ContentValues values) {
    Log.d(TAG, "Update: table name[" + table_name + "], row id[" + id + "], values[" + values + "]");
    int affected_number = mDbHandler.update(table_name, values, "ID = '" + id + "'", null);
    if (affected_number == 0) {
      Log.d(TAG, "Nothing to be updated.");
      return false;
    } else if (affected_number > 1) {
      String message = "More than one rows have been affected with update. This is invalid behavior.";
      Log.e(TAG, message);
      throw new RuntimeException(message);
    }
    return true;
  }
  
  private boolean delete(final String table_name, final long id) {
    Log.d(TAG, "Delete: table name[" + table_name + "], row id[" + id + "]");
    int affected_number = mDbHandler.delete(table_name, "ID = '" + id + "'", null);
    if (affected_number == 0) {
      Log.d(TAG, "No rows were deleted.");
      return false;
    }
    return true;
  }
  
  private boolean clearTable(final String table_name) {
    Log.d(TAG, "Clear: table name[" + table_name + "]");
    int affected_number = mDbHandler.delete(table_name, "1", null);
    if (affected_number == 0) {
      Log.d(TAG, "No rows were deleted.");
      return false;
    }
    return true;
  }
  
  private long totalRows(final String table_name) {
    Log.d(TAG, "Total rows: table name[" + table_name + "]");
    String statement = "SELECT COUNT(*) FROM '" + table_name + "'";
    Cursor cursor = mDbHandler.rawQuery(statement, null);
    if (cursor.moveToFirst()) {
      long rows_total = cursor.getLong(0);
      Log.d(TAG, "Number of rows in table " + table_name + " is " + rows_total);
      return rows_total;
    } else {
      String message = "Table " + table_name + " in database " + databaseName + " is empty!";
      Log.e(TAG, message);
      return 0L;
    }
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
