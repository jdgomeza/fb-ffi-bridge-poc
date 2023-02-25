#![feature(test)]
use std::{f32, slice};
extern crate flatbuffers;

extern crate test;

#[allow(dead_code, unused_imports)]
#[path = "./player_events_generated.rs"]
mod player_events_generated;

use crate::events::{PlayerMoved, PlayerMovedArgs};
pub use player_events_generated::my_game::events;

#[no_mangle]
pub extern "C" fn distance_points(x1: f32, y1: f32, z1: f32, x2: f32, y2: f32, z2: f32) -> f32 {
    return f32::sqrt(f32::powi(x2 - x1, 2) + f32::powi(y2 - y1, 2) + f32::powi(z2 - z1, 2));
}

#[no_mangle]
pub extern "C" fn calculate_event_distance(buf: *const u8, len: usize) -> f32 {
    let buf = unsafe {
        assert!(!buf.is_null());
        slice::from_raw_parts(buf, len)
    };
    calculate_distance(buf)
}

fn calculate_distance(buf: &[u8]) -> f32 {
    let event = events::root_as_player_event(buf).unwrap();
    if event.event_type() == events::Event::PlayerMoved {
        let event = event.event_as_player_moved().unwrap();
        let pos1 = event.pos1().unwrap();
        let pos2 = event.pos2().unwrap();
        return distance_points(pos1.x(), pos1.y(), pos1.z(), pos2.x(), pos2.y(), pos2.z());
    }
    return 0.0;
}

pub fn create_player_moved<'a, 'b>(
    fbb: &'b mut flatbuffers::FlatBufferBuilder<'a>,
) -> flatbuffers::WIPOffset<PlayerMoved<'a>> {
    let pos1 = events::Point3D::new(1.0, 2.0, 3.0);
    let pos2 = events::Point3D::new(1.5, 2.0, 3.0);
    let uuid = events::UUID::new(12u64, 23u64);
    events::PlayerMoved::create(
        fbb,
        &PlayerMovedArgs {
            player_id: Some(&uuid),
            pos1: Some(&pos1),
            pos2: Some(&pos2),
        },
    )
}

#[cfg(test)]
mod tests {

    use std::borrow::BorrowMut;

    use super::*;
    use crate::events::PlayerEventArgs;
    use test::Bencher;

    #[test]
    fn distance() {
        let result = distance_points(1.0, 2.0, 3.0, 1.0, 2.0, 3.0);
        assert_eq!(result, 0.0);
        let result = distance_points(1.0, 2.0, 3.0, 1.5, 2.0, 3.0);
        assert_eq!(result, 0.5);
        let (x1, x2) = (2.45, -1.38456);
        let result = distance_points(x1, 2.0, 3.0, x2, 2.0, 3.0);
        assert_eq!(result, f32::powi(x2 - x1, 2).sqrt());
    }

    #[test]
    fn movement_distance() {
        let mut fbb = flatbuffers::FlatBufferBuilder::with_capacity(1024);
        let player_moved = create_player_moved(fbb.borrow_mut());
        let event = events::PlayerEvent::create(
            fbb.borrow_mut(),
            &PlayerEventArgs {
                ts: 12u64,
                event_type: events::Event::PlayerMoved,
                event: Some(player_moved.as_union_value()),
            },
        );

        fbb.finish(event, None);
        let buf = fbb.finished_data();

        let dist = calculate_distance(buf);
        assert_eq!(dist, 0.5);
    }

    #[bench]
    fn bench_create_movement_event_reuse(b: &mut Bencher) {
        let mut fbb = flatbuffers::FlatBufferBuilder::with_capacity(1024);

        b.iter(|| {
            fbb.reset();
            let event = create_player_moved(&mut fbb);
            fbb.finish(event, None);
            fbb.finished_data();
        });
    }

    #[bench]
    fn bench_create_movement_event(b: &mut Bencher) {
        b.iter(|| {
            let mut fbb = flatbuffers::FlatBufferBuilder::with_capacity(1024);
            let event = create_player_moved(&mut fbb);
            fbb.finish(event, None);
            fbb.finished_data();
        });
    }

    #[bench]
    fn bench_slice_from_raw(b: &mut Bencher) {
        let mut fbb = flatbuffers::FlatBufferBuilder::with_capacity(1024);
        let moved = create_player_moved(&mut fbb);
        let event = events::PlayerEvent::create(
            &mut fbb,
            &PlayerEventArgs {
                ts: 12u64,
                event_type: events::Event::PlayerMoved,
                event: Some(moved.as_union_value()),
            },
        );

        fbb.finish(event, None);
        let buf = fbb.finished_data().as_ptr();
        let len = fbb.finished_data().len();
        let buf = unsafe {
            assert!(!buf.is_null());
            slice::from_raw_parts(buf, len)
        };

        b.iter(|| {
            let dist = calculate_distance(buf);
            assert_eq!(dist, 0.5);
        });
    }
}
