#include <pebble.h>

static StatusBarLayer *s_status_bar;
static TextLayer *first_word_layer;
static TextLayer *second_word_layer;
static TextLayer *third_word_layer;
static Layer *s_canvas_layer;
static TextLayer *latitude_text_layer;
static TextLayer *longitude_text_layer;
static GFont word_font;
static GFont  word_font_small;
static GFont cordinates_font;
static GRect bounds;
static ActionBarLayer *s_action_bar;
static GBitmap *refresh_button_bitmap;
Window* window;



static void canvas_update_proc(Layer *layer, GContext *ctx) {
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_stroke_width(ctx, 1);
    graphics_draw_line(ctx, GPoint(0, 130), GPoint(110,130));
}

void send_int(uint8_t key, uint8_t cmd)
{
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
 
    Tuplet value = TupletInteger(key, cmd);
    dict_write_tuplet(iter, &value);
 
    app_message_outbox_send();
}


static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "select click worked!");
  
  //set font size to a small enough size to fit on the screen in the case of longer words.
  
//   layer_set_hidden((Layer*) first_word_layer, true);
  text_layer_set_text(first_word_layer, " ");
  

  text_layer_set_text(second_word_layer, "  Loading...");
  
//   layer_set_hidden((Layer*) third_word_layer, true);
    text_layer_set_text(third_word_layer, " ");
  
    text_layer_set_text(latitude_text_layer, " ");
    text_layer_set_text(longitude_text_layer, " ");
  
//   // Begin dictionary
//   DictionaryIterator *iter;
//   app_message_outbox_begin(&iter);

//   // Add a key-value pair
//   dict_write_uint8(iter, 0, 0, 0);

//   // Send the message!
//   app_message_outbox_send();
  
  send_int(5, 5);
  
  layer_set_hidden((Layer*) first_word_layer, false);
  
  text_layer_set_text_alignment(second_word_layer, GTextAlignmentLeft);
  
  layer_set_hidden((Layer*) third_word_layer, false);
  
}


static void click_config_provider(void *context) {
  // Subcribe to button click events here
  
  ButtonId id = BUTTON_ID_SELECT;  // The Select button

  window_single_click_subscribe(id, select_click_handler);
}

 
void window_load(Window *window)
{
  
   window_set_background_color(window, GColorBlack);
   bounds = layer_get_bounds(window_get_root_layer(window));
  
      // Create first word Layer
    first_word_layer = text_layer_create(
        GRect(0, PBL_IF_ROUND_ELSE(20, 15), bounds.size.w, 40));
    
    // Style the text
    text_layer_set_background_color(first_word_layer, GColorClear);
    text_layer_set_text_color(first_word_layer, GColorWhite);
    text_layer_set_text_alignment(first_word_layer, GTextAlignmentLeft);
//     text_layer_set_text(first_word_layer, "Loading...");
  
  
        // Create second Layer
    second_word_layer = text_layer_create(
        GRect(0, PBL_IF_ROUND_ELSE(55, 50), bounds.size.w, 40));
  
      // Style the text
    text_layer_set_background_color(second_word_layer, GColorClear);
    text_layer_set_text_color(second_word_layer, GColorWhite);
    text_layer_set_text_alignment(second_word_layer, GTextAlignmentLeft);
    text_layer_set_text(second_word_layer, "    Loading...");
  
  
        // Create third word Layer
    third_word_layer = text_layer_create(
        GRect(0, PBL_IF_ROUND_ELSE(90, 85), bounds.size.w, 40));
  
  
      // Style the text
    text_layer_set_background_color(third_word_layer, GColorClear);
    text_layer_set_text_color(third_word_layer, GColorWhite);
    text_layer_set_text_alignment(third_word_layer, GTextAlignmentLeft);
//     text_layer_set_text(third_word_layer, "Loading...");
      
  
  
        // Create latitude text Layer
    latitude_text_layer = text_layer_create(
        GRect(7, PBL_IF_ROUND_ELSE(135, 130), bounds.size.w, 40));
        // Style the text
    text_layer_set_background_color(latitude_text_layer, GColorClear);
    text_layer_set_text_color(latitude_text_layer, GColorWhite);
    text_layer_set_text_alignment(latitude_text_layer, GTextAlignmentLeft);
  
  
  
  
          // Create longitude text Layer
    longitude_text_layer = text_layer_create(
        GRect(60, PBL_IF_ROUND_ELSE(135, 130), bounds.size.w, 40));
          // Style the text
    text_layer_set_background_color(longitude_text_layer, GColorClear);
    text_layer_set_text_color(longitude_text_layer, GColorWhite);
    text_layer_set_text_alignment(longitude_text_layer, GTextAlignmentLeft);
  
  
  
  
  
    // Create two fonts (for lat/lon and the three words) and apply it
    word_font = fonts_get_system_font(FONT_KEY_GOTHIC_28);
    word_font_small =  fonts_get_system_font(FONT_KEY_GOTHIC_24);
    text_layer_set_font(first_word_layer, word_font);
    text_layer_set_font(second_word_layer, word_font);
    text_layer_set_font(third_word_layer, word_font);
    cordinates_font = fonts_get_system_font(FONT_KEY_GOTHIC_14);
    text_layer_set_font(latitude_text_layer,  cordinates_font);
    text_layer_set_font(longitude_text_layer, cordinates_font);
 
  
    // and text layers to the windows;
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(first_word_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(second_word_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(third_word_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(latitude_text_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(longitude_text_layer));

  
      // Load icon bitmaps
    refresh_button_bitmap = gbitmap_create_with_resource(RESOURCE_ID_REFRESH_ICON);
  
    // Create ActionBarLayer
    s_action_bar = action_bar_layer_create();
    action_bar_layer_set_click_config_provider(s_action_bar, click_config_provider);
    action_bar_layer_set_background_color(s_action_bar, GColorWhite);
  
  // Set the icons
  action_bar_layer_set_icon(s_action_bar, BUTTON_ID_SELECT, refresh_button_bitmap);
  
    // Add to Window action bar
    action_bar_layer_add_to_window(s_action_bar, window);
  
  
  
  // Create canvas layer
    s_canvas_layer = layer_create(bounds);
  // Assign the custom drawing procedure
    layer_set_update_proc(s_canvas_layer, canvas_update_proc);
    // Add to Window
//   layer_add_child(window_get_root_layer(window), s_canvas_layer);
  
  

}
 


void window_unload(Window *window)
{
  // Destroy weather elements
  text_layer_destroy(first_word_layer);
  
  //unload the fonts
  fonts_unload_custom_font(word_font);
  
  
//  Destroy the ActionBarLayer
 action_bar_layer_destroy(s_action_bar);
}


static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information in the following buffers
  static char word_one_buffer[32];
  static char word_two_buffer[32];
  static char word_three_buffer[32];
  static char lat_characteristic_buffer[32];
  static char lat_mantissa_buffer[32];
  static char lon_characteristic_buffer[32];
  static char lon_mantissa_buffer[32];
  static char less_than_ten_buffer[32];

  // create two buffers for combining the characteristic and mantissa buffers into final form to be applied to latitude and longitude text layers
  static char lattitude_layer_buffer[32];
  static char longitude_layer_buffer[32];  
    
    // Read tuples for data
  Tuple *word_one_tuple = dict_find(iterator, MESSAGE_KEY_WORD_ONE);
  Tuple *word_two_tuple = dict_find(iterator, MESSAGE_KEY_WORD_TWO);
  Tuple *word_three_tuple = dict_find(iterator, MESSAGE_KEY_WORD_THREE);
  Tuple *latitude_characteristic_tuple = dict_find(iterator, MESSAGE_KEY_LAT_CHARACTERISTIC);
  Tuple *latitude_mantissa_tuple = dict_find(iterator, MESSAGE_KEY_LAT_MANTISSA);
  Tuple *longitude_characteristic_tuple = dict_find(iterator, MESSAGE_KEY_LON_CHARACTERISTIC);
  Tuple *longitude_mantissa_tuple = dict_find(iterator, MESSAGE_KEY_LON_MANTISSA);
  Tuple *less_than_ten_tuple = dict_find(iterator, MESSAGE_KEY_LESS_THAN_TEN);
  


  // If all data is available, use it
  if(word_one_tuple&&word_two_tuple&&word_three_tuple&&latitude_characteristic_tuple&&latitude_mantissa_tuple&&longitude_characteristic_tuple&&longitude_mantissa_tuple) {
    
    
    // read the text for the three words into three respective buffers
    snprintf(word_one_buffer, sizeof(word_one_buffer), "%s ", word_one_tuple->value->cstring);
    snprintf(word_two_buffer, sizeof(word_two_buffer), "%s ", word_two_tuple->value->cstring);
    snprintf(word_three_buffer, sizeof(word_three_buffer), "%s ", word_three_tuple->value->cstring);

    // read the text for the lat and lon characteristic and mantissa values into their respective buffers
    snprintf(lat_characteristic_buffer, sizeof(lat_characteristic_buffer), "%s ", latitude_characteristic_tuple->value->cstring);
    snprintf(lat_mantissa_buffer, sizeof(lat_mantissa_buffer), "%s ", latitude_mantissa_tuple->value->cstring);
    snprintf(lon_characteristic_buffer, sizeof(lon_characteristic_buffer), "%s ", longitude_characteristic_tuple->value->cstring);
    snprintf(lon_mantissa_buffer, sizeof(lon_mantissa_buffer), "%s ", longitude_mantissa_tuple->value->cstring);
    
        // set text for the three words and align them if necessary
       text_layer_set_text(first_word_layer, word_one_buffer);
    
       text_layer_set_text(second_word_layer, word_two_buffer);
       text_layer_set_text_alignment(second_word_layer, GTextAlignmentLeft);

       text_layer_set_text(third_word_layer, word_three_buffer);
    
    //if there are more than ten characters in one of the three words then set the font of all three texts to smalelr values
     snprintf(less_than_ten_buffer, sizeof(less_than_ten_buffer), "%d ", (int)less_than_ten_tuple->value->int32);
     APP_LOG(APP_LOG_LEVEL_INFO, "less_than_ten_buffer = %s", less_than_ten_buffer);    
     if((int)less_than_ten_tuple->value->int32 == 1) {
//        text_layer_set_font(first_word_layer, word_font);
//        text_layer_set_font(second_word_layer, word_font);
//        text_layer_set_font(third_word_layer, word_font);
      text_layer_set_font(first_word_layer, word_font_small);
      text_layer_set_font(second_word_layer, word_font_small);
      text_layer_set_font(third_word_layer, word_font_small);
     }
    else  {
//       text_layer_set_font(first_word_layer, word_font_small);
//       text_layer_set_font(second_word_layer, word_font_small);
//       text_layer_set_font(third_word_layer, word_font_small);
        text_layer_set_font(first_word_layer, word_font);
       text_layer_set_font(second_word_layer, word_font);
       text_layer_set_font(third_word_layer, word_font);
    }
    
    
      // join into two respective layers the lattitude and longitude characteristic and mantissa touples. 
       snprintf(lattitude_layer_buffer, sizeof(lattitude_layer_buffer), "     Lat\n%s.%s", lat_characteristic_buffer, lat_mantissa_buffer);
       snprintf(longitude_layer_buffer, sizeof(longitude_layer_buffer), "     Lon\n%s.%s", lon_characteristic_buffer, lon_mantissa_buffer);
    
      // set the text for the lat and lon layers
        text_layer_set_text(latitude_text_layer, lattitude_layer_buffer);
        text_layer_set_text(longitude_text_layer, longitude_layer_buffer);
    
        // Add to Window canvas(for thin line seperating the three words and lat/lon cordinates) and the lat and lon text layers.
        layer_add_child(window_get_root_layer(window), s_canvas_layer);
//         layer_add_child(window_get_root_layer(window), text_layer_get_layer(latitude_text_layer));
//         layer_add_child(window_get_root_layer(window), text_layer_get_layer(longitude_text_layer));
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}
 
void init()
{
  window = window_create();
  
  // Use this provider to add button click subscriptions
  window_set_click_config_provider(window, click_config_provider);
  WindowHandlers handlers = {
    .load = window_load,
    .unload = window_unload
  };
  window_set_window_handlers(window, handlers);
 
   // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  // Open AppMessage
  const int inbox_size = 128;
  const int outbox_size = 128;
  app_message_open(inbox_size, outbox_size);  
  
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Create the StatusBarLayer
s_status_bar = status_bar_layer_create();

// Set properties
status_bar_layer_set_colors(s_status_bar, GColorClear, GColorWhite);
status_bar_layer_set_separator_mode(s_status_bar, 
                                            StatusBarLayerSeparatorModeNone);

int16_t width = layer_get_bounds(window_get_root_layer(window)).size.w - ACTION_BAR_WIDTH;
GRect frame = GRect(0, 0, width, STATUS_BAR_LAYER_HEIGHT);
layer_set_frame(status_bar_layer_get_layer(s_status_bar), frame);

// Add the status bar to the window
layer_add_child(window_get_root_layer(window), status_bar_layer_get_layer(s_status_bar));
 
// push the window onto the stack
window_stack_push(window, true);
}
 
void deinit()
{
  // Destroy the StatusBarLayer
status_bar_layer_destroy(s_status_bar);


  // Destroy the window
  window_destroy(window);
  
}
 
int main(void)
{
  init();
  app_event_loop();
  deinit();
}