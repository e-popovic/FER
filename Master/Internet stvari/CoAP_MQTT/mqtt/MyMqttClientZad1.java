package mqtt;

import org.eclipse.paho.client.mqttv3.MqttClient;
        import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
        import org.eclipse.paho.client.mqttv3.MqttException;
        import org.eclipse.paho.client.mqttv3.MqttMessage;
        import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

public class MyMqttClientZad1 {
	//PUBLISHER 
	
	static String temp = parserTemp("<=>€##408521538#node_01#0#TCA:26.13#");
	static String hum = parserHum("<=>€##408521538#node_01#0#HUMA:48#");
	
	static public String parserTemp(String nonparsed) {
    	return nonparsed.substring(nonparsed.indexOf(":")+1, nonparsed.indexOf(":")+6);
    }
    static public String parserHum(String nonparsed) {
    	return nonparsed.substring(nonparsed.indexOf(":")+1, nonparsed.indexOf(":")+3);
    }

    public static void main(String[] args) {

        String topictemp        = "mqtt-spy/temp";
        String topichum         = "mqtt-spy/hum";
        int qos             = 2;	//  po potrebi promijeni vrijednost (0-1-2)
        String broker       = "tcp://127.0.0.1:1883";	// localhost s mosquitto portom
        String clientId     = "0036506085";
        MemoryPersistence persistence = new MemoryPersistence();

        try {
            MqttClient myClient = new MqttClient(broker, clientId, persistence);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setCleanSession(true);
            System.out.println("Connecting to broker: "+broker);
            myClient.connect(connOpts);
            System.out.println("Connected");
            
            //temperatura
            System.out.println("Publishing temperature: "+temp);
            MqttMessage message = new MqttMessage(temp.getBytes());
            message.setQos(qos);
            myClient.publish(topictemp, message);
            System.out.println("Temperature published\n");

            //vlaga
            System.out.println("Publishing humidity: "+hum);
            MqttMessage message2 = new MqttMessage(hum.getBytes());
            message2.setQos(qos);
            myClient.publish(topichum, message2);
            System.out.println("Humidity published\n");
            
            myClient.disconnect();
            System.out.println("Disconnected");
            System.exit(0);
        } catch(MqttException me) {
            System.out.println("reason "+me.getReasonCode());
            System.out.println("msg "+me.getMessage());
            System.out.println("loc "+me.getLocalizedMessage());
            System.out.println("cause "+me.getCause());
            System.out.println("excep "+me);
            me.printStackTrace();
        }
    }
    
}