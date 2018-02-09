#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define GAME_TERMINATE -1

// ALLEGRO Variables
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_BITMAP *image = NULL;
ALLEGRO_BITMAP *image2 = NULL;
ALLEGRO_BITMAP *image3 = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_KEYBOARD_STATE keyState ;
ALLEGRO_TIMER *timerClock = NULL;
ALLEGRO_TIMER *timerJump = NULL;
ALLEGRO_TIMER *timerWeapon = NULL;
ALLEGRO_SAMPLE *song=NULL;
ALLEGRO_SAMPLE *songFight=NULL;
ALLEGRO_FONT *font = NULL;

//Custom Definition
const char *title = "Final Project 1061044s";
const float FPS = 60;
const int WIDTH = 800;
const int HEIGHT = 600;
typedef struct character
{
    int x;
    int y;
    ALLEGRO_BITMAP *image_path;
    
}Character;

Character character1;
Character character2;
Character character3;

Character character1Weapon;
Character character2Weapon;

int clock = 10;
bool clockMode = false;

bool character1WeaponFlying = false;
bool character2WeaponFlying = false;

int character1JumpingState = 0;
int character2JumpingState = 0;

int character1HP = 100;
int character2HP = 100;

int winner = 0;

int imageWidth = 128;
int imageHeight = 128;
int draw = 0;
int done = 0;
int window = 0;
bool judge_next_window = false;
int next_window = 2;
bool ture_1 , ture_2, ture_Weapon;

char img1_location[100] = "dragon.png";
char img2_location[100] = "ninja.png";
char img3_location[100] = "wizard.png";
char weapon1_location[100] = "fireball.png";
char weapon2_location[100] = "dart.png";
char weapon3_location[100] = "lightning.png";
char *p1_img = img1_location;
char *p2_img = img2_location;
char *p1_weapon_img = weapon1_location;
char *p2_weapon_img = weapon2_location;

void show_err_msg(int msg);
void game_init();
void game_begin();
int process_event();
int game_run();
void game_destroy();
void display_window0(int, int);
void display_window1();
void display_window3();
void display_window4();
void display_window5();
void display_window6();
void setup_characters();
void create_and_play_music2();
void stop_and_destroy_music2();
void start_animation();

int main(int argc, char *argv[]) {
    int msg = 0;
    
    game_init();
    game_begin();
    
    while (msg != GAME_TERMINATE) {
        msg = game_run();
        if (msg == GAME_TERMINATE)
            printf("Game Over\n");
    }
    
    game_destroy();
    return 0;
}

void show_err_msg(int msg) {
    fprintf(stderr, "unexpected msg: %d\n", msg);
    game_destroy();
    exit(9);
}

void game_init() {
    if (!al_init()) {
        show_err_msg(-1);
    }
    if(!al_install_audio()){
        fprintf(stderr, "failed to initialize audio!\n");
        show_err_msg(-1);
    }
    if(!al_init_acodec_addon()){
        fprintf(stderr, "failed to initialize audio codecs!\n");
        show_err_msg(-1);
    }
    if (!al_reserve_samples(1)){
        fprintf(stderr, "failed to reserve samples!\n");
        show_err_msg(-1);
    }
    // Create display
    display = al_create_display(WIDTH, HEIGHT);
    event_queue = al_create_event_queue();
    if (display == NULL || event_queue == NULL) {
        show_err_msg(-1);
    }
    // Initialize Allegro settings
    al_set_window_position(display, 0, 0);
    al_set_window_title(display, title);
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_image_addon();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    
    // Display window #0
    display_window0(0, 255);
    al_rest(0.5);
    display_window0(0, 0);
    al_rest(0.5);
    display_window0(0, 255);
    al_rest(0.5);
    display_window0(0, 0);
    al_rest(0.5);
    display_window0(0, 255);
    al_rest(0.5);
    display_window0(0, 0);
    al_rest(1);
    window = 1;
    
    // Register event
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
}

void game_begin() {
    // Load sound
    song = al_load_sample( "hello.wav" );
    if (!song){
        printf( "Audio clip sample not loaded!\n" );
        show_err_msg(-1);
    }
    
    // Loop the song until the display closes
    al_play_sample(song, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    
    // Display window #1
    display_window1();
}

int process_event(){
    // Request the event
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    
    // Our setting for controlling animation
    if(event.timer.source == timerClock){
        clock -= 1;
    }
    
    if(event.timer.source == timerJump){
        switch (character1JumpingState) {
            case 0:
                break;
            case 1:
                character1JumpingState = 2;
                character1.y -= imageHeight/2;
                if (!character1WeaponFlying) character1Weapon.y = character1.y + imageHeight/2;
                break;
            case 2:
                character1JumpingState = 3;
                character1.y -= imageHeight/2;
                if (!character1WeaponFlying) character1Weapon.y = character1.y + imageHeight/2;
                break;
            case 3:
                character1JumpingState = 4;
                character1.y += imageHeight/2;
                if (!character1WeaponFlying) character1Weapon.y = character1.y + imageHeight/2;
                break;
            case 4:
                character1JumpingState = 0;
                character1.y += imageHeight/2;
                if (!character1WeaponFlying) character1Weapon.y = character1.y + imageHeight/2;
                break;
            default:
                break;
        }
        switch (character2JumpingState) {
            case 0:
                break;
            case 1:
                character2JumpingState = 2;
                character2.y -= imageHeight/2;
                if (!character2WeaponFlying) character2Weapon.y = character2.y + imageHeight/2;
                break;
            case 2:
                character2JumpingState = 3;
                character2.y -= imageHeight/2;
                if (!character2WeaponFlying) character2Weapon.y = character2.y + imageHeight/2;
                break;
            case 3:
                character2JumpingState = 4;
                character2.y += imageHeight/2;
                if (!character2WeaponFlying) character2Weapon.y = character2.y + imageHeight/2;
                break;
            case 4:
                character2JumpingState = 0;
                character2.y += imageHeight/2;
                if (!character2WeaponFlying) character2Weapon.y = character2.y + imageHeight/2;
                break;
            default:
                break;
        }
    }
    
    if(event.timer.source == timerWeapon){
        ture_Weapon = !ture_Weapon ;
        
        if (character1WeaponFlying) {
            if (character1.x < character2.x) {
                if (character1Weapon.x > WIDTH) {
                    character1WeaponFlying = false;
                    character1Weapon.x = character1.x + imageWidth/2;
                    character1Weapon.y = character1.y + imageHeight/2;
                }
                else {
                    character1Weapon.x += imageWidth/2;
                }
            }
            else {
                if (character1Weapon.x < 0) {
                    character1WeaponFlying = false;
                    character1Weapon.x = character1.x + imageWidth/2;
                    character1Weapon.y = character1.y + imageHeight/2;
                }
                else {
                    character1Weapon.x -= imageWidth/2;
                }
            }
            
            // Object Collision Detection
            if (character1Weapon.x >=  character2.x && character1Weapon.x <=  character2.x+imageWidth && character1Weapon.y == character2.y+imageHeight/2) {
                character2HP -= 25;
                
                character1WeaponFlying = false;
                character1Weapon.x = character1.x + imageWidth/2;
                character1Weapon.y = character1.y + imageHeight/2;
            }
        }
        if (character2WeaponFlying) {
            if (character2.x > character1.x) {
                if (character2Weapon.x < 0) {
                    character2WeaponFlying = false;
                    character2Weapon.x = character2.x + imageWidth/2;
                    character2Weapon.y = character2.y + imageHeight/2;
                }
                else {
                    character2Weapon.x -= imageWidth/2;
                }
            }
            else {
                if (character2Weapon.x > WIDTH) {
                    character2WeaponFlying = false;
                    character2Weapon.x = character2.x + imageWidth/2;
                    character2Weapon.y = character2.y + imageHeight/2;
                }
                else {
                    character2Weapon.x += imageWidth/2;
                }
            }
            
            // Object Collision Detection
            if (character2Weapon.x >=  character1.x && character2Weapon.x <=  character1.x+imageWidth && character2Weapon.y == character1.y+imageHeight/2) {
                character1HP -= 25;
                
                character2WeaponFlying = false;
                character2Weapon.x = character2.x + imageWidth/2;
                character2Weapon.y = character2.y + imageHeight/2;
            }
        }
    }
    
    // Keyboard
    if(event.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch(event.keyboard.keycode)
        {
                // P1 control
            case ALLEGRO_KEY_W:
                if (character1JumpingState == 0) character1JumpingState = 1;
                /*if (character1.y - 30 >= 0) character1.y -= 30;
                 if (!character1WeaponFlying) character1Weapon.y = character1.y + 64;*/
                break;
                /*case ALLEGRO_KEY_S:
                 if (character1.y + 30 <= HEIGHT-100) character1.y += 30;
                 if (!character1WeaponFlying) character1Weapon.y = character1.y + 64;
                 break;*/
            case ALLEGRO_KEY_A:
                if (character1.x - 30 >= 0) character1.x -= 30;
                if (!character1WeaponFlying) character1Weapon.x = character1.x + imageWidth/2;
                break;
            case ALLEGRO_KEY_D:
                if (character1.x + 30 <= WIDTH-100) character1.x += 30;
                if (!character1WeaponFlying) character1Weapon.x = character1.x + imageWidth/2;
                break;
            case ALLEGRO_KEY_X:
                if (!character1WeaponFlying && character1JumpingState == 0) character1WeaponFlying = true;
                break;
                
                // P2 control
            case ALLEGRO_KEY_UP:
                if (character2JumpingState == 0) character2JumpingState = 1;
                /*if (character2.y - 30 >= 0) character2.y -= 30;
                 if (!character2WeaponFlying) character2Weapon.y = character2.y + 64;*/
                break;
                /*case ALLEGRO_KEY_DOWN:
                 if (character2.y + 30 <= HEIGHT-100) character2.y += 30;
                 if (!character2WeaponFlying) character2Weapon.y = character2.y + 64;
                 break;*/
            case ALLEGRO_KEY_RIGHT:
                if (character2JumpingState == 0) {
                    if (character2.x + 30 <= WIDTH-100) character2.x += 30;
                    if (!character2WeaponFlying) character2Weapon.x = character2.x + imageWidth/2;
                    
                }
                break;
            case ALLEGRO_KEY_LEFT:
                if (character2JumpingState == 0) {
                    if (character2.x - 30 >= 0) character2.x -= 30;
                    if (!character2WeaponFlying) character2Weapon.x = character2.x + imageWidth/2;
                    
                }
                break;
            case ALLEGRO_KEY_Y:
                if (!character2WeaponFlying && character2JumpingState == 0) character2WeaponFlying = true;
                break;
                
                // For Start or Return Menu
            case ALLEGRO_KEY_ENTER:
                judge_next_window = true;
                switch (window) {
                    case 1:
                        // Start
                        next_window = 2;
                        break;
                    case 3:
                        // Return
                        next_window = 1;
                        break;
                    case 4:
                        // Restart
                        next_window = 2;
                        break;
                    case 5:
                        // Return
                        next_window = 1;
                        break;
                    case 6:
                        // Return
                        next_window = 1;
                        break;
                    default:
                        break;
                }
                break;
                
                // For Exit Menu
            case ALLEGRO_KEY_ESCAPE:
                if (window == 1 || window == 4) return GAME_TERMINATE;
                break;
                
                // For About or ChooseCharacter Menu
            case ALLEGRO_KEY_1:
                if (window == 1) {
                    judge_next_window = true;
                    next_window = 3;
                }
                else if (window == 5) {
                    p1_img = img1_location;
                    p1_weapon_img = weapon1_location;
                    display_window5();
                }
                else if(window == 6){
                    clockMode = false;
                    display_window6();
                }
                break;
            case ALLEGRO_KEY_2:
                if (window == 1) {
                    judge_next_window = true;
                    next_window = 5;
                }
                else if (window == 5) {
                    p1_img = img2_location;
                    p1_weapon_img = weapon2_location;
                    display_window5();
                }
                else if(window == 6){
                    clockMode = true;
                    display_window6();
                }
                break;
            case ALLEGRO_KEY_3:
                if (window == 1) {
                    judge_next_window = true;
                    next_window = 6;
                }
                else if (window == 5) {
                    p1_img = img3_location;
                    p1_weapon_img = weapon3_location;
                    display_window5();
                }
                break;
            case ALLEGRO_KEY_4:
                if (window == 5) {
                    p2_img = img1_location;
                    p2_weapon_img = weapon1_location;
                    display_window5();
                }
                break;
            case ALLEGRO_KEY_5:
                if (window == 5) {
                    p2_img = img2_location;
                    p2_weapon_img = weapon2_location;
                    display_window5();
                }
                break;
            case ALLEGRO_KEY_6:
                if (window == 5) {
                    p2_img = img3_location;
                    p2_weapon_img = weapon3_location;
                    display_window5();
                }
                break;
        }
    }
    
    // Shutdown our program
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return GAME_TERMINATE;
    
    return 0;
}

int game_run() {
    int error = 0;
    // First window(Menu)
    if(window == 1){
        if (!al_is_event_queue_empty(event_queue)) {
            error = process_event();
            if(judge_next_window) {
                window = next_window;
                judge_next_window = false;
                switch (next_window) {
                    case 2:
                        // Change Music
                        al_destroy_sample(song);
                        create_and_play_music2();
                        
                        // Setup Characters
                        setup_characters();
                        
                        //Initialize Timer
                        timerClock = al_create_timer(1.0);
                        timerJump = al_create_timer(1.0/10.0);
                        timerWeapon = al_create_timer(1.0/10.0);
                        al_register_event_source(event_queue, al_get_timer_event_source(timerClock));
                        al_register_event_source(event_queue, al_get_timer_event_source(timerJump));
                        al_register_event_source(event_queue, al_get_timer_event_source(timerWeapon));
                        al_start_timer(timerClock);
                        al_start_timer(timerJump);
                        al_start_timer(timerWeapon);
                        break;
                    case 3:
                        display_window3();
                        break;
                    case 5:
                        display_window5();
                        break;
                    case 6:
                        display_window6();
                        break;
                    default:
                        break;
                }
            }
        }
    }
    // Second window(Main Game)
    else if(window == 2){
        if (judge_next_window) {
            window = next_window;
            judge_next_window = false;
            switch (next_window) {
                case 4:
                    display_window4();
                    break;
                default:
                    break;
            }
        }
        else {
            if (clockMode && clock == 0) {
                winner = 0;
                judge_next_window = true;
                next_window = 4;
                stop_and_destroy_music2();
            }
            if (character1HP <= 0 || character2HP <=0 ) {
                if (character1HP <= 0) winner = 2;
                else winner = 1;
                
                judge_next_window = true;
                next_window = 4;
                stop_and_destroy_music2();
            }
            else {
                // Change Image for animation
                al_draw_bitmap(background, 0, 0, 0);
                al_draw_bitmap(character1.image_path, character1.x, character1.y, 0);
                al_draw_bitmap(character2.image_path, character2.x, character2.y, 0);
                
                // Draw clock
                if (clockMode) {
                    char clockstr[2] = "";
                    sprintf(clockstr, "%d", clock);
                    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, 20, ALLEGRO_ALIGN_CENTRE, clockstr);
                }
                
                // Draw HP bar
                al_draw_filled_rectangle(10, 10, 10 + character1HP, 20, al_map_rgb(255, 0, 0));
                al_draw_filled_rectangle(10, 50, 10 + character2HP, 60, al_map_rgb(255, 0, 0));
                
                // Draw weapon
                if(ture_Weapon) {
                    al_draw_bitmap(character1Weapon.image_path, character1Weapon.x, character1Weapon.y, 0);
                    al_draw_bitmap(character2Weapon.image_path, character2Weapon.x, character2Weapon.y, 0);
                }
                
                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
                
                // Listening for new event
                if (!al_is_event_queue_empty(event_queue)) {
                    error = process_event();
                }
            }
        }
    }
    else if (window == 3){
        if (!al_is_event_queue_empty(event_queue)) {
            error = process_event();
            if(judge_next_window) {
                window = next_window;
                judge_next_window = false;
                switch (next_window) {
                    case 1:
                        display_window1();
                        break;
                    default:
                        break;
                }
            }
        }
    }
    else if (window == 4){
        if(judge_next_window) {
            window = next_window;
            judge_next_window = false;
            switch (next_window) {
                case 2:
                    setup_characters();
                    create_and_play_music2();
                    break;
                default:
                    break;
            }
        }
        else {
            // Listening for new event
            if (!al_is_event_queue_empty(event_queue)) {
                error = process_event();
            }
        }
    }
    else if (window == 5){
        if(judge_next_window) {
            window = next_window;
            judge_next_window = false;
            switch (next_window) {
                case 1:
                    display_window1();
                    break;
                default:
                    break;
            }
        }
        else {
            // Listening for new event
            if (!al_is_event_queue_empty(event_queue)) {
                error = process_event();
            }
        }
    }
    else if (window == 6){
        if(judge_next_window) {
            window = next_window;
            judge_next_window = false;
            switch (next_window) {
                case 1:
                    display_window1();
                    break;
                default:
                    break;
            }
        }
        else {
            // Listening for new event
            if (!al_is_event_queue_empty(event_queue)) {
                error = process_event();
            }
        }
    }
    return error;
}

void game_destroy() {
    // Make sure you destroy all things
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(timerClock);
    al_destroy_timer(timerJump);
    al_destroy_timer(timerWeapon);
    al_destroy_bitmap(image);
    if (window == 2) al_destroy_sample(songFight);
    else if (window == 1 || window == 3) al_destroy_sample(song);
}

void display_window0(int bg_color, int font_color) {
    // Clear
    al_clear_to_color(al_map_rgb(bg_color, bg_color, bg_color));
    
    // Load and draw text
    font = al_load_ttf_font("pirulen.ttf",50,0);
    al_draw_text(font, al_map_rgb(font_color, font_color, font_color), WIDTH/2, HEIGHT/2 , ALLEGRO_ALIGN_CENTRE, "Allegro");
    al_flip_display();
}

void display_window1() {
    // Clear
    al_clear_to_color(al_map_rgb(100,100,100));
    
    // Load and draw text
    font = al_load_ttf_font("pirulen.ttf",12,0);
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+20 , ALLEGRO_ALIGN_CENTRE, "Press '1' to about");
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+70 , ALLEGRO_ALIGN_CENTRE, "Press '2' to choose character");
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+120 , ALLEGRO_ALIGN_CENTRE, "Press '3' to choose mode");
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+170 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to start");
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+220 , ALLEGRO_ALIGN_CENTRE, "Press 'Esc' to exit");
    al_draw_rectangle(200, 310, 600, 350, al_map_rgb(255, 255, 255), 0);
    al_draw_rectangle(200, 360, 600, 400, al_map_rgb(255, 255, 255), 0);
    al_draw_rectangle(200, 410, 600, 450, al_map_rgb(255, 255, 255), 0);
    al_draw_rectangle(200, 460, 600, 500, al_map_rgb(255, 255, 255), 0);
    al_draw_rectangle(200, 510, 600, 550, al_map_rgb(255, 255, 255), 0);
    al_flip_display();
}

void display_window3() {
    // Clear
    al_clear_to_color(al_map_rgb(100,100,100));
    
    // Load and draw text
    font = al_load_ttf_font("pirulen.ttf",12,0);
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+20 , ALLEGRO_ALIGN_CENTRE, "About");
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+70 , ALLEGRO_ALIGN_CENTRE, "Author: 1061044s");
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+220 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to return");
    al_draw_rectangle(200, 250, 600, 450, al_map_rgb(255, 255, 255), 0);
    al_draw_rectangle(200, 510, 600, 550, al_map_rgb(255, 255, 255), 0);
    al_flip_display();
}

void display_window4() {
    // Clear
    al_clear_to_color(al_map_rgb(100,100,100));
    
    // Load and draw text
    font = al_load_ttf_font("pirulen.ttf",12,0);
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+20 , ALLEGRO_ALIGN_CENTRE, "Game Over");
    if (winner == 1) al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+70 , ALLEGRO_ALIGN_CENTRE, "Winner: P1");
    else if (winner == 2) al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+70 , ALLEGRO_ALIGN_CENTRE, "Winner: P2");
    else al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+70 , ALLEGRO_ALIGN_CENTRE, "Time's Up. No Winner");
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+170 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to restart");
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+220 , ALLEGRO_ALIGN_CENTRE, "Press 'Esc' to end");
    al_draw_rectangle(200, 250, 600, 450, al_map_rgb(255, 255, 255), 0);
    al_draw_rectangle(200, 460, 600, 500, al_map_rgb(255, 255, 255), 0);
    al_draw_rectangle(200, 510, 600, 550, al_map_rgb(255, 255, 255), 0);
    al_flip_display();
}

void display_window5() {
    // Clear
    al_clear_to_color(al_map_rgb(100,100,100));
    
    // Setting Character
    character1.x = WIDTH / 2 - 300;
    character1.y = HEIGHT / 2 + 45;
    character2.x = WIDTH / 2 - 65;
    character2.y = HEIGHT / 2 + 45;
    character3.x = WIDTH / 2 + 170;
    character3.y = HEIGHT / 2 + 45;
    
    al_draw_bitmap(al_load_bitmap(img1_location), character1.x, character1.y, 0);
    al_draw_bitmap(al_load_bitmap(img2_location), character2.x, character2.y, 0);
    al_draw_bitmap(al_load_bitmap(img3_location), character3.x, character3.y, 0);
    
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, character1.y - 50 , ALLEGRO_ALIGN_CENTRE, "Press '1' '2' '3' to change P1. Press '4' '5' '6' to change P2");
    
    if (p1_img == img1_location) {
        al_draw_text(font, al_map_rgb(0,128,255), character1.x + imageWidth/2, character1.y - 10 , ALLEGRO_ALIGN_CENTRE, "1");
        al_draw_text(font, al_map_rgb(255,255,255), character2.x + imageWidth/2, character1.y - 10 , ALLEGRO_ALIGN_CENTRE, "2");
        al_draw_text(font, al_map_rgb(255,255,255), character3.x + imageWidth/2, character1.y - 10 , ALLEGRO_ALIGN_CENTRE, "3");
    }
    else if (p1_img == img2_location) {
        al_draw_text(font, al_map_rgb(255,255,255), character1.x + imageWidth/2, character1.y - 10 , ALLEGRO_ALIGN_CENTRE, "1");
        al_draw_text(font, al_map_rgb(0,128,255), character2.x + imageWidth/2, character1.y - 10 , ALLEGRO_ALIGN_CENTRE, "2");
        al_draw_text(font, al_map_rgb(255,255,255), character3.x + imageWidth/2, character1.y - 10 , ALLEGRO_ALIGN_CENTRE, "3");
    }
    else {
        al_draw_text(font, al_map_rgb(255,255,255), character1.x + imageWidth/2, character1.y - 10 , ALLEGRO_ALIGN_CENTRE, "1");
        al_draw_text(font, al_map_rgb(255,255,255), character2.x + imageWidth/2, character1.y - 10 , ALLEGRO_ALIGN_CENTRE, "2");
        al_draw_text(font, al_map_rgb(0,128,255), character3.x + imageWidth/2, character1.y - 10 , ALLEGRO_ALIGN_CENTRE, "3");
    }
    
    if (p2_img == img1_location) {
        al_draw_text(font, al_map_rgb(0,128,255), character1.x + imageWidth/2, character1.y + imageHeight + 10 , ALLEGRO_ALIGN_CENTRE, "4");
        al_draw_text(font, al_map_rgb(255,255,255), character2.x + imageWidth/2, character1.y + imageHeight + 10 , ALLEGRO_ALIGN_CENTRE, "5");
        al_draw_text(font, al_map_rgb(255,255,255), character3.x + imageWidth/2, character1.y + imageHeight + 10 , ALLEGRO_ALIGN_CENTRE, "6");
    }
    else if (p2_img == img2_location) {
        al_draw_text(font, al_map_rgb(255,255,255), character1.x + imageWidth/2, character1.y + imageHeight + 10 , ALLEGRO_ALIGN_CENTRE, "4");
        al_draw_text(font, al_map_rgb(0,128,255), character2.x + imageWidth/2, character1.y + imageHeight + 10 , ALLEGRO_ALIGN_CENTRE, "5");
        al_draw_text(font, al_map_rgb(255,255,255), character3.x + imageWidth/2, character1.y + imageHeight + 10 , ALLEGRO_ALIGN_CENTRE, "6");
    }
    else {
        al_draw_text(font, al_map_rgb(255,255,255), character1.x + imageWidth/2, character1.y + imageHeight + 10 , ALLEGRO_ALIGN_CENTRE, "4");
        al_draw_text(font, al_map_rgb(255,255,255), character2.x + imageWidth/2, character1.y + imageHeight + 10 , ALLEGRO_ALIGN_CENTRE, "5");
        al_draw_text(font, al_map_rgb(0,128,255), character3.x + imageWidth/2, character1.y + imageHeight + 10 , ALLEGRO_ALIGN_CENTRE, "6");
    }
    
    al_draw_text(font, al_map_rgb(255,255,255), character1.x - 10, character1.y - 10 , ALLEGRO_ALIGN_CENTRE, "P1");
    al_draw_text(font, al_map_rgb(255,255,255), character1.x - 10, character1.y + imageHeight + 10 , ALLEGRO_ALIGN_CENTRE, "P2");
    
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+220 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to return");
    al_draw_rectangle(200, 510, 600, 550, al_map_rgb(255, 255, 255), 0);
    
    al_flip_display();
}

void display_window6() {
    // Clear
    al_clear_to_color(al_map_rgb(100,100,100));
    
    // Load and draw text
    font = al_load_ttf_font("pirulen.ttf",12,0);
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+70, ALLEGRO_ALIGN_CENTRE, "Press '1' '2' to change mode.");
    if (clockMode) {
        al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+120, ALLEGRO_ALIGN_CENTRE, "1. Normal Mode");
        al_draw_text(font, al_map_rgb(0,128,255), WIDTH/2, HEIGHT/2+170, ALLEGRO_ALIGN_CENTRE, "2. Clock Mode");
    }
    else {
        al_draw_text(font, al_map_rgb(0,128,255), WIDTH/2, HEIGHT/2+120, ALLEGRO_ALIGN_CENTRE, "1. Normal Mode");
        al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+170, ALLEGRO_ALIGN_CENTRE, "2. Clock Mode");
    }
    al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+220 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to return");
    al_draw_rectangle(200, 510, 600, 550, al_map_rgb(255, 255, 255), 0);
    al_flip_display();
}

void setup_characters() {
    // Setting Character
    character1.x = WIDTH / 2 - 300;
    character1.y = HEIGHT / 2 + 45;
    character2.x = WIDTH / 2 + 170;
    character2.y = HEIGHT / 2 + 45;
    character1.image_path = al_load_bitmap(p1_img);
    character2.image_path= al_load_bitmap(p2_img);
    background = al_load_bitmap("stage.jpg");
    
    clock = 10;
    character1HP = 100;
    character2HP = 100;
    
    // Setting Character's Weapon
    character1Weapon.x = character1.x + imageWidth/2;
    character1Weapon.y = character1.y + imageHeight/2;
    character1Weapon.image_path = al_load_bitmap(p1_weapon_img);
    character2Weapon.x = character2.x + imageWidth/2;
    character2Weapon.y = character2.y + imageHeight/2;
    character2Weapon.image_path = al_load_bitmap(p2_weapon_img);
}

void create_and_play_music2() {
    songFight = al_load_sample( "fight.wav" );
    if (!songFight){
        printf( "Audio clip sample not loaded!\n" );
        show_err_msg(-1);
    }
    al_play_sample(songFight, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
}

void stop_and_destroy_music2() {
    al_destroy_sample(songFight);
}
