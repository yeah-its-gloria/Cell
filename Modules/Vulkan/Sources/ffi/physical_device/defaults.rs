// SPDX-FileCopyrightText: Copyright 2023 Gloria G.
// SPDX-License-Identifier: BSD-2-Clause

use crate::ffi::{
    Extent3D, MemoryHeap, MemoryHeapFlags, MemoryType, PhysicalDeviceLimits, PhysicalDeviceProperties, PhysicalDeviceSparseProperties, PhysicalDeviceType, QueueFamilyProperties, QueueFlags, SampleCount, VK_FALSE
};

use super::PhysicalDeviceMemoryProperties;

impl Default for PhysicalDeviceLimits {
    fn default() -> Self {
        PhysicalDeviceLimits {
            max_image_dimension_1d: 0,
            max_image_dimension_2d: 0,
            max_image_dimension_3d: 0,
            max_image_dimension_cube: 0,
            max_image_array_layers: 0,
            max_texel_buffer_elements: 0,
            max_uniform_buffer_range: 0,
            max_storage_buffer_range: 0,
            max_push_constants_size: 0,
            max_memory_allocation_count: 0,
            max_sampler_allocation_count: 0,
            buffer_image_granularity: 0,
            sparse_address_space_size: 0,
            max_bound_descriptor_sets: 0,
            max_per_stage_descriptor_samplers: 0,
            max_per_stage_descriptor_uniform_buffers: 0,
            max_per_stage_descriptor_storage_buffers: 0,
            max_per_stage_descriptor_sampled_images: 0,
            max_per_stage_descriptor_storage_images: 0,
            max_per_stage_descriptor_input_attachments: 0,
            max_per_stage_resources: 0,
            max_descriptor_set_samplers: 0,
            max_descriptor_set_uniform_buffers: 0,
            max_descriptor_set_uniform_buffers_dynamic: 0,
            max_descriptor_set_storage_buffers: 0,
            max_descriptor_set_storage_buffers_dynamic: 0,
            max_descriptor_set_sampled_images: 0,
            max_descriptor_set_storage_images: 0,
            max_descriptor_set_input_attachments: 0,
            max_vertex_input_attributes: 0,
            max_vertex_input_bindings: 0,
            max_vertex_input_attribute_offset: 0,
            max_vertex_input_binding_stride: 0,
            max_vertex_output_components: 0,
            max_tessellation_generation_level: 0,
            max_tessellation_patch_size: 0,
            max_tessellation_control_per_vertex_input_components: 0,
            max_tessellation_control_per_vertex_output_components: 0,
            max_tessellation_control_per_patch_output_components: 0,
            max_tessellation_control_total_output_components: 0,
            max_tessellation_evaluation_input_components: 0,
            max_tessellation_evaluation_output_components: 0,
            max_geometry_shader_invocations: 0,
            max_geometry_input_components: 0,
            max_geometry_output_components: 0,
            max_geometry_output_vertices: 0,
            max_geometry_total_output_components: 0,
            max_fragment_input_components: 0,
            max_fragment_output_attachments: 0,
            max_fragment_dual_src_attachments: 0,
            max_fragment_combined_output_resources: 0,
            max_compute_shared_memory_size: 0,
            max_compute_work_group_count: [0; 3],
            max_compute_work_group_invocations: 0,
            max_compute_work_group_size: [0; 3],
            sub_pixel_precision_bits: 0,
            sub_texel_precision_bits: 0,
            mipmap_precision_bits: 0,
            max_draw_indexed_index_value: 0,
            max_draw_indirect_count: 0,
            max_sampler_lod_bias: 0.0,
            max_sampler_anisotropy: 0.0,
            max_viewports: 0,
            max_viewport_dimensions: [0; 2],
            viewport_bounds_range: [0.0; 2],
            viewport_sub_pixel_bits: 0,
            min_memory_map_alignment: 0,
            min_texel_buffer_offset_alignment: 0,
            min_uniform_buffer_offset_alignment: 0,
            min_storage_buffer_offset_alignment: 0,
            min_texel_offset: 0,
            max_texel_offset: 0,
            min_texel_gather_offset: 0,
            max_texel_gather_offset: 0,
            min_interpolation_offset: 0.0,
            max_interpolation_offset: 0.0,
            sub_pixel_interpolation_offset_bits: 0,
            max_framebuffer_width: 0,
            max_framebuffer_height: 0,
            max_framebuffer_layers: 0,
            framebuffer_color_sample_counts: SampleCount::None,
            framebuffer_depth_sample_counts: SampleCount::None,
            framebuffer_stencil_sample_counts: SampleCount::None,
            framebuffer_no_attachments_sample_counts: SampleCount::None,
            max_color_attachments: 0,
            sampled_image_color_sample_counts: SampleCount::None,
            sampled_image_integer_sample_counts: SampleCount::None,
            sampled_image_depth_sample_counts: SampleCount::None,
            sampled_image_stencil_sample_counts: SampleCount::None,
            storage_image_sample_counts: SampleCount::None,
            max_sample_mask_words: 0,
            timestamp_compute_and_graphics: VK_FALSE,
            timestamp_period: 0.0,
            max_clip_distances: 0,
            max_cull_distances: 0,
            max_combined_clip_and_cull_distances: 0,
            discrete_queue_priorities: 0,
            point_size_range: [0.0; 2],
            line_width_range: [0.0; 2],
            point_size_granularity: 0.0,
            line_width_granularity: 0.0,
            strict_lines: VK_FALSE,
            standard_sample_locations: VK_FALSE,
            optimal_buffer_copy_offset_alignment: 0,
            optimal_buffer_copy_row_pitch_alignment: 0,
            non_coherent_atom_size: 0,
        }
    }
}

impl Default for PhysicalDeviceSparseProperties {
    fn default() -> Self {
        PhysicalDeviceSparseProperties {
            residency_standard_2d_block_shape: VK_FALSE,
            residency_standard_2d_multisample_block_shape: VK_FALSE,
            residency_standard_3d_block_shape: VK_FALSE,
            residency_aligned_mip_size: VK_FALSE,
            residency_non_resident_strict: VK_FALSE,
        }
    }
}

impl Default for PhysicalDeviceProperties {
    fn default() -> Self {
        PhysicalDeviceProperties {
            api_version: 0,
            driver_version: 0,
            vendor_id: 0,
            device_id: 0,
            device_type: PhysicalDeviceType::Other,
            device_name: [0; 256],
            pipeline_cache_uuid: [0; 16],
            limits: PhysicalDeviceLimits::default(),
            sparse_properties: PhysicalDeviceSparseProperties::default(),
        }
    }
}

impl Default for QueueFamilyProperties {
    fn default() -> Self {
        QueueFamilyProperties {
            flags: QueueFlags::from_bits_retain(0),
            count: 0,
            timestamp_valid_bits: 0,
            minimal_image_transfer_granularity: Extent3D { width: 0, height: 0, depth: 0 },
        }
    }
}

impl Default for PhysicalDeviceMemoryProperties {
    fn default() -> Self {
        PhysicalDeviceMemoryProperties {
            memory_type_count: 0,
            memory_types: [MemoryType { property_flags: 0, heap_index: 0 }; 16],
            memory_heap_count: 0,
            memory_heaps: [MemoryHeap { size: 0, flags: MemoryHeapFlags::None }; 16],
        }
    }
}
