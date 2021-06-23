package mqtt;

import java.sql.Timestamp;
import java.util.concurrent.CountDownLatch;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

public class MyMqttClientZad3 {

    public static void main(String[] args) {
    	//SUBSCRIBER

        String broker = "tcp://127.0.0.1:1883";
        String clientId = "Popovic";
        int qos             = 0;	//  po potrebi promijeni vrijednost (0-1-2)
        String topictemp        = "mqtt-spy/temp";
        String topichum         = "mqtt-spy/hum";
        MemoryPersistence persistence = new MemoryPersistence();

        try {
        	MqttClient myClient = new MqttClient(broker, clientId, persistence);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setCleanSession(true);
            System.out.println("Connecting to broker: "+broker);
            myClient.connect(connOpts);
            System.out.println("Connected");
            final CountDownLatch latch = new CountDownLatch(1);
            
            myClient.setCallback(new MqttCallback() {
                public void messageArrived(String topic, MqttMessage message) throws Exception {
                    String time = new Timestamp(System.currentTimeMillis()).toString();
                    System.out.println("\nReceived a Message!" +
                            "\n\tTime:    " + time + 
                            "\n\tTopic:   " + topic + 
                            "\n\tMessage: " + new String(message.getPayload()) + 
                            "\n\tQoS:     " + message.getQos() + "\n");
                    latch.countDown();
                }

                public void connectionLost(Throwable cause) {
                    System.out.println("Connection lost!" + cause.getMessage());
                    latch.countDown();
                }

                public void deliveryComplete(IMqttDeliveryToken token) {}

            });
            
            System.out.println("Subscribing to topic: " + topictemp);
            myClient.subscribe(topictemp, qos);
            System.out.println("Subscribing to topic: " + topichum);
            myClient.subscribe(topichum, qos);
            System.out.println("Subscribed");

            try {
                latch.await();
            } catch (InterruptedException e) {
                System.out.println("I was awoken while waiting");
            }
            
            myClient.disconnect();
            System.out.println("Exiting");
            System.exit(0);
        } catch (MqttException me) {
            System.out.println("reason " + me.getReasonCode());
            System.out.println("msg " + me.getMessage());
            System.out.println("loc " + me.getLocalizedMessage());
            System.out.println("cause " + me.getCause());
            System.out.println("excep " + me);
            me.printStackTrace();
        }
    }

}
