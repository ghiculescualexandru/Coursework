import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

/**
 * Main class.
 * 
 * @author Ghiculescu Alexandru
 *
 */
public class Main {
	/**
	 * Static variable representing the number of seconds in a day.
	 */
	private static final long ONE_DAY = 86400;
	/**
	 * Static variable representing the number of seconds in an hour.
	 */
	private static final int ONE_HOUR = 3600;

	public static void main(String[] args) throws IOException {

		File file = new File("./therm.in");
		
		Scanner sc = new Scanner(file);

		FileWriter fw = null;
		try {
			fw = new FileWriter("./therm.out");
		} catch (Exception e) {
			System.out.println(e);
		}

		String firstLine = sc.nextLine();
		String firstLineArr[] = firstLine.split(" ");

		int numberOfRooms; /* The number of rooms. */
		double globalTemperature; /* The global temperature for trigger heat. */
		double globalHumidity; /* The gloobal humidity for trigger heat. */
		long globalTimestamp; /* The reference timestamp. */

		/*
		 * If there is a global humidity, it will be read and used in the HOUSE
		 * constructor.
		 */
		if (firstLineArr.length == 4) {
			numberOfRooms = Integer.parseInt(firstLineArr[0]);
			String globalTemperatureBuffer = firstLineArr[1];
			globalTemperature = Double.parseDouble(globalTemperatureBuffer);
			String globalHumidityBuffer = firstLineArr[2];
			globalHumidity = Double.parseDouble(globalHumidityBuffer);
			globalTimestamp = Long.parseLong(firstLineArr[3]);
			/* Otherwise, the global humidity will be set as 0. */
		} else {
			numberOfRooms = Integer.parseInt(firstLineArr[0]);
			String globalTemperatureBuffer = firstLineArr[1];
			globalTemperature = Double.parseDouble(globalTemperatureBuffer);
			globalHumidity = 0;
			globalTimestamp = Long.parseLong(firstLineArr[2]);
		}

		/* Constructing the house with the given arguments. */
		House house = new House(numberOfRooms, globalTemperature, globalHumidity, globalTimestamp);

		/* Reading and adding the rooms into the rooms array for the house. */
		int cnt = numberOfRooms;
		while (cnt > 0) {
			String buffer = sc.nextLine();
			String arrayBuffer[] = buffer.split(" ");

			String roomName = arrayBuffer[0];
			String deviceId = arrayBuffer[1];
			String areaBuffer = arrayBuffer[2];
			int area = Integer.parseInt(areaBuffer);

			Room room = new Room(roomName, deviceId, area);
			house.addRoom(room);

			cnt--;
		}

		/* Reading the actions to be done. */
		while (sc.hasNext()) {
			String buffer = sc.nextLine();
			String action[] = buffer.split(" ");

			switch (action[0]) {

			case "OBSERVE":
				String idToObserve = action[1];
				long timestamp = Long.parseLong(action[2]);
				double temperature = Double.parseDouble(action[3]);
				/*
				 * The difference between the reference timpestamp and the timestamp to be
				 * observed.
				 */
				long diff = globalTimestamp - timestamp;

				/*
				 * If the difference is higher then the number of seconds in a day OR negative,
				 * it means that the observation won't be considered.
				 */
				if (diff > ONE_DAY || diff < 0) {
					break;
				}

				/*
				 * This is the hour where the observation will be added. Basically, if the hour
				 * is 0, it means the observation will be added in the last 3600 seconds. If the
				 * hour is 1, the observation will be added between 7200 seconds ago and 3601
				 * seconds ago. And so on.
				 */
				int hour = (int) (diff / ONE_HOUR);

				/*
				 * If the difference is exactly 3600/7200/etc seconds, the hour will be 0/1/etc
				 * .
				 */
				if (diff % 3600 == 0) {
					hour -= 1;
				}
				/* Constructing the temperature observation with the given parameters. */
				TemperatureObservation temperatureObservation = new TemperatureObservation(timestamp, temperature);
				house.addTemperatureObservation(hour, temperatureObservation, idToObserve);
				break;

			case "OBSERVEH":
				idToObserve = action[1];
				timestamp = Long.parseLong(action[2]);
				double humidity = Double.parseDouble(action[3]);
				/*
				 * The difference between the reference timpestamp and the timestamp to be
				 * observed.
				 */
				diff = globalTimestamp - timestamp;

				/*
				 * If the difference is higher then the number of seconds in a day OR negative,
				 * it means that the observation won't be considered.
				 */
				if (diff > ONE_DAY || diff < 0) {
					break;
				}

				/*
				 * This is the hour where the observation will be added. Basically, if the hour
				 * is 0, it means the observation will be added in the last 3600 seconds. If the
				 * hour is 1, the observation will be added between 7200 seconds ago and 3601
				 * seconds ago. And so on.
				 */
				hour = (int) (diff / ONE_HOUR);

				/*
				 * If the difference is exactly 3600/7200/etc seconds, the hour will be 0/1/etc
				 * .
				 */
				if (diff % 3600 == 0) {
					hour -= 1;
				}

				/* Constructing the humidity observation with the given parameters. */
				HumidityObservation humidityObservation = new HumidityObservation(timestamp, humidity);
				house.addHumidityObservation(hour, humidityObservation, idToObserve);
				break;

			case "LIST":
				String roomToList = action[1];
				long startTime = Long.parseLong(action[2]);
				long stopTime = Long.parseLong(action[3]);
				/* Writing the temperature for the list operation. */
				house.list(fw, Character.getNumericValue(roomToList.charAt(4)), startTime, stopTime);
				break;

			case "TRIGGER":
				/* Writing the result of trigger heat command. */
				fw.append(house.triggerHeat() + "\n");
				break;

			case "TEMPERATURE":
				double newTemperature = Double.parseDouble(action[1]);
				/* Setting the new global temperature. */
				house.setGlobalTemperature(newTemperature);
				break;
			}

		}
		fw.close();
		sc.close();
	}

}
