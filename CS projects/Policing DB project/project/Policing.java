import java.sql.*;
import java.text.DecimalFormat;
import java.util.Scanner;

public class Policing {

	private static Connection conn;
	private static DecimalFormat df;
	
	public static void main(String argus[]){
		conn = null;
		df = new DecimalFormat("#0.00");
        try {
            // db parameters
            String url = "jdbc:sqlite:C:/Users/Raud/Desktop/Database/Policing.db";
            // create a connection to the database
            conn = DriverManager.getConnection(url);
            System.out.println("Connection established.");
            Scanner in =  new Scanner(System.in);
            System.out.println("********************************Please select an Option**********************************");
            System.out.println("0: Exit");
            System.out.println("1: Find the cities with the highest black arrest rate");
            System.out.println("2: Find the cities with the highest white arrest rates");
            System.out.println("3: Shows the amount of cities with broken windows policing");
            System.out.println("4: Shows the amount of cities with stop and frisk policing");
            System.out.println("5: Shows the cities with a higher black arrest rate than the average and showing their policing strategies");
            System.out.println("6: Shows the cities with a higher black arrest that don't enforce either policy, if they have a higher black arrest rate than the average of cities that do have both policies");
            System.out.println("*****************************************************************************************");
            int input = in.nextInt();
            int num;
            while(input != 0){
            	if(input == 1){
            		System.out.print("How many cities would you like to show?: ");
            		num = in.nextInt();
            		Query1(num);
            	} else if(input  == 2){
            		System.out.print("How many cities would you like to show?: ");
            		num = in.nextInt();
            		Query2(num);
            	} else if(input == 3){
            		Query3();
            	} else if(input == 4){
            		Query4();
            	} else if(input == 5){
            		Query5();
            	} else if(input == 6){
            		Query6();
            	}
            	System.out.println("********************************Please select an Option**********************************");
                System.out.println("0: Exit");
                System.out.println("1: Find the cities with the highest black arrest rate");
                System.out.println("2: Find the cities with the highest white arrest rates");
                System.out.println("3: Shows the amount of cities with broken windows policing");
                System.out.println("4: Shows the amount of cities with stop and frisk policing");
                System.out.println("5: Shows the cities with a higher black arrest rate than the average and showing their policing strategies");
                System.out.println("6: Shows the cities with a higher black arrest that don't enforce either policy, if they have a higher black arrest rate than the average of cities that do have both policies");
                System.out.println("*****************************************************************************************");
                input = in.nextInt();
            }
            conn.close();
            System.out.println("Connection closed.");
        } catch (SQLException e) {
            System.out.println(e.getMessage());
        }
	}
	public static void Query1(int num) throws SQLException{
		System.out.println("Executing query 1:\n");
		String sql = "SELECT CityName , BlackRate, WhiteRate "
					+ "FROM ArrestRates NATURAL JOIN City NATURAL JOIN PoliceDepartment NATURAL JOIN PolicingStrategies "
					+ "ORDER BY BlackRate DESC "
					+ "LIMIT ?";

		PreparedStatement pstmt  = conn.prepareStatement(sql);
		pstmt.setInt(1,num);
		// set the value
		ResultSet rs  = pstmt.executeQuery();

		// loop through the result set
		while (rs.next()) {
			String cityname = rs.getString("CityName");
			int blackrate = rs.getInt("BlackRate");
			int whiterate = rs.getInt("WhiteRate");
			System.out.println("City: " + cityname +  ", Black Rate: " + blackrate + ", White Rate: " + whiterate);
			String percent = df.format(blackrate / (double)whiterate);
			System.out.println("You are approximately " + percent + " times as likely to get arrested in " + cityname + " if you are black.\n");
		}
	}
	public static void Query2(int num) throws SQLException{
		System.out.println("Executing query 2:\n");
		String sql = "SELECT CityName , BlackRate, WhiteRate "
					+ "FROM ArrestRates NATURAL JOIN City NATURAL JOIN PoliceDepartment NATURAL JOIN PolicingStrategies "
					+ "ORDER BY WhiteRate DESC "
					+ "LIMIT ?";

		PreparedStatement pstmt  = conn.prepareStatement(sql);
		pstmt.setInt(1,num);
		// set the value
		ResultSet rs  = pstmt.executeQuery();

		// loop through the result set
		while (rs.next()) {
			String cityname = rs.getString("CityName");
			int blackrate = rs.getInt("BlackRate");
			int whiterate = rs.getInt("WhiteRate");
			System.out.println("City: " + cityname +  ", Black Rate: " + blackrate + ", White Rate: " + whiterate);
			String percent = df.format((whiterate/(double)blackrate));
			System.out.println("You are approximately " + percent + " times as likely to get arrested in " + cityname + " if you are white.\n");
		}
		
	}
	public static void Query3() throws SQLException{
		System.out.println("Executing query 3:\n");
		String sql = "SELECT count(*) "
					+ "FROM ArrestRates NATURAL JOIN City NATURAL JOIN PoliceDepartment NATURAL JOIN PolicingStrategies "
					+ "WHERE PolicingStrategies.BrokenWindows = true";

		PreparedStatement pstmt  = conn.prepareStatement(sql);
		// set the value
		ResultSet rs  = pstmt.executeQuery();

		// loop through the result set
		while (rs.next()) {
			System.out.println("The amount of cities with broken windows policing: " + rs.getString("count(*)") + "\n");
		}
		
	}
	public static void Query4() throws SQLException{
		System.out.println("Executing query 4:\n");
		String sql = "SELECT count(*) "
					+ "FROM ArrestRates NATURAL JOIN City NATURAL JOIN PoliceDepartment NATURAL JOIN PolicingStrategies "
					+ "WHERE PolicingStrategies.StopAndFrisk = true";

		PreparedStatement pstmt  = conn.prepareStatement(sql);
		// set the value
		ResultSet rs  = pstmt.executeQuery();

		// loop through the result set
		while (rs.next()) {
			System.out.println("The amount of cities with stop and frisk policing: " + rs.getString("count(*)")+ "\n");
		}
		
	}
	public static void Query5() throws SQLException{
		System.out.println("Executing query 5:\n");
		String sql = "SELECT CityName, BrokenWindows, StopAndFrisk "
					+ "FROM ArrestRates NATURAL JOIN City NATURAL JOIN PoliceDepartment NATURAL JOIN PolicingStrategies "
					+ "WHERE ArrestRates.BlackRate > (SELECT avg(BlackRate)"
                               + "FROM ArrestRates NATURAL JOIN City NATURAL JOIN PoliceDepartment NATURAL JOIN PolicingStrategies)"
                               + "ORDER BY ArrestRates.BlackRate DESC";

		PreparedStatement pstmt  = conn.prepareStatement(sql);
		// set the value
		ResultSet rs  = pstmt.executeQuery();

		// loop through the result set
		while (rs.next()) {
			System.out.println("City Name: " + rs.getString("CityName") + ", Broken Windows: " + rs.getBoolean("BrokenWindows") + ", Stop and Frisk: " + rs.getBoolean("StopAndFrisk"));
		}
		System.out.println();
	}
	public static void Query6() throws SQLException{
		System.out.println("Executing query 6:\n");
		String sql = "SELECT CityName "
					+ "FROM ArrestRates NATURAL JOIN City NATURAL JOIN PoliceDepartment NATURAL JOIN PolicingStrategies "
					+ "WHERE( PolicingStrategies.StopAndFrisk = false) AND  (PolicingStrategies.BrokenWindows = false) AND ArrestRates.BlackRate > (SELECT avg(BlackRate) "
					+ "FROM ArrestRates NATURAL JOIN City NATURAL JOIN PoliceDepartment NATURAL JOIN PolicingStrategies "
					+ "WHERE PolicingStrategies.StopAndFrisk = true AND PolicingStrategies.BrokenWindows = true)";

		PreparedStatement pstmt  = conn.prepareStatement(sql);
		// set the value
		ResultSet rs  = pstmt.executeQuery();

		// loop through the result set
		while (rs.next()) {
			System.out.println("City: " + rs.getString("CityName"));
		}
		System.out.println();
	}
}
