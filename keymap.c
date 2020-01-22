#include QMK_KEYBOARD_H
extern keymap_config_t keymap_config;

typedef struct{
	bool is_press_action;
	int state;	
} tap;

enum custom_keycodes {KC_PASTA = SAFE_RANGE,
	KC_PSWD1,
};

enum {
	SINGLE_TAP = 1,
	SINGLE_HOLD = 2,
	DOUBLE_TAP = 3,
	DOUBLE_HOLD = 4,
	DOUBLE_SINGLE_TAP = 5,
	TRIPLE_TAP = 6,
	TRIPLE_HOLD = 7
};	

enum{
	SHIFT_CTL = 0,
};

int cur_dance (qk_tap_dance_state_t *state);

void sc_finished (qk_tap_dance_state_t *state, void *user_data);
void sc_reset (qk_tap_dance_state_t *state, void *user_data);


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(
		KC_GESC, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_QUOT, KC_BSPC, 
		CTL_T(KC_TAB), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, LT(1, KC_ENT), 
		TD(SHIFT_CTL), KC_Z, KC_X, KC_C, KC_V, LT(2, KC_B), LT(2, KC_SPC), KC_N, KC_M, ALT_T(KC_COMM), GUI_T(KC_DOT), CTL_T(KC_SLSH), KC_RSPC),

	[1] = LAYOUT(
		KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSLS, KC_DEL, 
		KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_PIPE, KC_MINS, KC_EQL, KC_NO, KC_TRNS, KC_TRNS, 
		KC_LSFT, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_TRNS, KC_LBRC, KC_RBRC, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

	[2] = LAYOUT(
		RESET, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DQUO, KC_F12, 
		KC_TRNS, KC_HOME, KC_UP, KC_END, KC_TRNS, KC_TRNS, KC_BSLS, KC_UNDS, KC_PLUS, KC_NO, KC_UP, KC_TRNS, 
		KC_PSWD1, KC_LEFT, KC_DOWN, KC_RIGHT, KC_PSCR, KC_TRNS, KC_TRNS, KC_LCBR, KC_RCBR, KC_NO, KC_LEFT, KC_DOWN, KC_RIGHT),

	[3] = LAYOUT(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    if (record->event.pressed) {
    switch (keycode) {
            case KC_PASTA:
                if (record->event.pressed) {
                    SEND_STRING("pasta");
                } else {}
                break;
			case KC_PSWD1:
                if (record->event.pressed) {
                    SEND_STRING("MyPassword");
                } else {}
                break;
           
        }
    }
    return true;
}	

void matrix_init_user(void) {
  // set CapsLock LED to output and low
  setPinOutput(B2);
  writePinLow(B2);
  // set NumLock LED to output and low
  setPinOutput(B6);
  writePinLow(B6);
  
}

void matrix_scan_user(void) {

};
	
uint32_t layer_state_set_user(uint32_t state)
{
    if (state & (1<<1)) {
    writePinHigh(B2);
    } else if (state & (1<<2)) {
        writePinLow(B2);
        writePinHigh(B6);
    } else if (state & (1<<3)) {
        writePinHigh(B2);
        writePinHigh(B6);
    } else {
        writePinLow(B2);
        writePinLow(B6);
    }
    return state;
};


int cur_dance (qk_tap_dance_state_t *state){
	if (state->count ==1) {
		if (state->interrupted || !state->pressed) return SINGLE_TAP;
		//key has not been interrupted, but they key is still held. Means you want to send a 'HOLD'.
		else return SINGLE_HOLD;
	}
	else if (state->count ==2) {
		if (state->interrupted) return DOUBLE_SINGLE_TAP;
		else if (state->pressed) return DOUBLE_HOLD;
		else return DOUBLE_TAP;
	}
	if (state->count ==3) {
		if (state->interrupted || !state->pressed) return TRIPLE_TAP;
		else return TRIPLE_HOLD;
	}
	else return 8;
};

static tap xtap_state = {
	.is_press_action = true,
	.state = 0
};

void sc_finished (qk_tap_dance_state_t *state, void *user_data) {
	xtap_state.state = cur_dance(state);
	switch (xtap_state.state) {
		case SINGLE_TAP: register_code(KC_LPRN); break;
		case SINGLE_HOLD: register_code(KC_LSFT); break;
		case DOUBLE_TAP: register_code(KC_RPRN); break;
		case DOUBLE_HOLD: register_code(KC_LCTL); 
	}
}

void sc_reset (qk_tap_dance_state_t *state, void *user_data){
	switch (xtap_state.state) {
		case SINGLE_TAP: unregister_code(KC_LPRN); break;
		case SINGLE_HOLD: unregister_code(KC_LSFT); break;
		case DOUBLE_TAP: unregister_code(KC_RPRN); break;
		case DOUBLE_HOLD: unregister_code(KC_LCTL);
	}
	xtap_state.state = 0;	
}

qk_tap_dance_state_t tap_dance_actions[] = {
[SHIFT_CTL] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, sc_finished, sc_reset)
};