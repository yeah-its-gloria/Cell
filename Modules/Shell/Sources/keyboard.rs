// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use core::fmt::{Display, Formatter, Result};

bitflags::bitflags! {
    /// Represents buttons on a keyboard.
    #[repr(C)]
    #[derive(Clone, Copy, Debug, PartialEq)]
    pub struct KeyboardButton : u64 {
        const None = 0;

        const Zero  = 1 << 0;
        const One   = 1 << 1;
        const Two   = 1 << 2;
        const Three = 1 << 3;
        const Four  = 1 << 4;
        const Five  = 1 << 5;
        const Six   = 1 << 6;
        const Seven = 1 << 7;
        const Eight = 1 << 8;
        const Nine  = 1 << 9;

        const A = 1 << 10;
        const B = 1 << 11;
        const C = 1 << 12;
        const D = 1 << 13;
        const E = 1 << 14;
        const F = 1 << 15;
        const G = 1 << 16;
        const H = 1 << 17;
        const I = 1 << 18;
        const J = 1 << 19;
        const K = 1 << 20;
        const L = 1 << 21;
        const M = 1 << 22;
        const N = 1 << 23;
        const O = 1 << 24;
        const P = 1 << 25;
        const Q = 1 << 26;
        const R = 1 << 27;
        const S = 1 << 28;
        const T = 1 << 29;
        const U = 1 << 30;
        const V = 1 << 31;
        const W = 1 << 32;
        const X = 1 << 33;
        const Y = 1 << 34;
        const Z = 1 << 35;

        const Escape       = 1 << 36;
        const Space        = 1 << 37;
        const Backspace    = 1 << 38;
        const Tab          = 1 << 39;
        const Return       = 1 << 40;
        const LeftShift    = 1 << 41;
        const RightShift   = 1 << 42;
        const LeftControl  = 1 << 43;
        const RightControl = 1 << 44;
        const LeftAlt      = 1 << 45;
        const RightAlt     = 1 << 46;

        const UpArrow    = 1 << 47;
        const DownArrow  = 1 << 48;
        const LeftArrow  = 1 << 49;
        const RightArrow = 1 << 50;

        const F1  = 1 << 51;
        const F2  = 1 << 52;
        const F3  = 1 << 53;
        const F4  = 1 << 54;
        const F5  = 1 << 55;
        const F6  = 1 << 56;
        const F7  = 1 << 57;
        const F8  = 1 << 58;
        const F9  = 1 << 59;
        const F10 = 1 << 60;
        const F11 = 1 << 61;
        const F12 = 1 << 62;
    }
}

impl Display for KeyboardButton {
    fn fmt(&self, f: &mut Formatter<'_>) -> Result {
        write!(f, "{}", self.bits())
    }
}
