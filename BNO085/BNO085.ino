/*
  BNO085

  Demo the Complex Art's Sensor Board BNO085 IMU.

*/

#include <ca_sensor.h>
#include <ca_bno085.h>

BNO085 bno085;

char buffer[256];

bool led_state = true;
int led_counter = 0;
const int led_cycle = 20;    

// the setup function runs once when you press reset or power the board
void setup() {

    // initialize serial
    Serial.begin(115200);
  
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(ULED1, OUTPUT);
    pinMode(ULED2, OUTPUT);

    // initialize BNO085
    if (bno085.begin() != 0) {
      Serial.println("Could not initialize BNO085");
      Serial.println("Going to sleep...");
      esp_deep_sleep_start();
    }

    // request product id
    ca_sh2_product_id_response_t product_id_response;
    bno085.get_product_id( product_id_response );

    Serial.println("BNO085 product id:");
    Serial.print("- reset cause:  ");
    Serial.println(product_id_response.reset_cause);
    Serial.print("- version: ");
    Serial.print(product_id_response.sw_version_major);
    Serial.print(".");
    Serial.println(product_id_response.sw_version_minor);
    Serial.print("- part #: ");
    Serial.println(product_id_response.sw_part_number);
    Serial.print("- build #: ");
    Serial.println(product_id_response.sw_build_number);
    Serial.print("- version patch: ");
    Serial.println(product_id_response.sw_version_patch);

    // set accelerometer feature
    FeatureCommand feature = FeatureCommand( SH2_SENSOR_ACCELEROMETER );
    feature.set_report_interval( 50000 );
    Serial.println(String(feature));
    
    bno085.set_feature_command( feature );
    Serial.println(String(feature));

    // set orientation feature
    feature = FeatureCommand( SH2_SENSOR_ROTATION_VECTOR );
    feature.set_report_interval( 50000 );
    Serial.println(String(feature));
    
    bno085.set_feature_command( feature );
    Serial.println(String(feature));

}

// the loop function runs over and over again forever
void loop() {

    ca_sh2_sensor_response_t response;
    if ( bno085.wait_sensor_response( response ) ) {

      ca_sh2_accelerometer_t accel = bno085.get_accelerometer();
      ca_sh2_rotation_vector_t rotation = bno085.get_rotation_vector();
      Quaternion quat = Quaternion(rotation);
      Euler euler = quat.toEuler();

      sprintf( buffer, "accel: x = %6.2f, y = %6.2f, z = %6.2f\t|\trot: roll = %6.1f, pitch = %6.1f, yaw = %6.1f", 
               accel.accel_x, accel.accel_y, accel.accel_z,
               RAD_TO_DEG*euler.getRoll(), RAD_TO_DEG*euler.getPitch(), RAD_TO_DEG*euler.getYaw() );
      Serial.println(buffer);

      // toggle leds
      led_state = (led_counter++ % led_cycle) == 0 ? !led_state : led_state;
      digitalWrite(ULED1, led_state);
      digitalWrite(ULED2, !led_state);
            
    }

}
